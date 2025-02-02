#include "ParserUtils.hh"
#include "Parser.hh"
#include "Patterns/Patterns.hh"
#include "Utils.hh"
#include "common/logger.hh"
#include <cstdint>
#include <stdexcept>
#include <stdio.h>
#include <string>

/*******************************************************/
template <auto &O>
void
ParserUtils::BuildStructureHook (parStructure *          structure,
                                 parStructureStaticData *data)
{
    m_pStaticDatas[data->Name] = data;
    O (structure, data);
}

/*******************************************************/
void
ParserUtils::Register ()
{
    static bool bRegistered = false;
    if (std::exchange (bRegistered, true))
        return;

    REGISTER_HOOK ("8b 4a 10 e8 ? ? ? ? ? 8b 43 10 ? 83 c4 20", 3,
                   BuildStructureHook, void, parStructure *,
                   parStructureStaticData *);
}

/*******************************************************/
parMemberCommonData *
ParserUtils::FindFieldData (parMemberCommonData **datas, uint32_t hash)
{
    while (*datas)
        {
            if ((*datas)->nHash == hash)
                return *datas;

            datas++;
        }

    return nullptr;
}

/*******************************************************/
void *
ParserUtils::FindFieldPtr (parStructureStaticData *data, void *ptr,
                           uint32_t hash)
{
    uint64_t             initialOffset = 0;
    parMemberCommonData *field         = FindFieldData (data->Params, hash);

    while (!field)
        {
            data = FindStaticData (data->Structure->pBaseClass);

            if (!data)
                {
                    Rainbomizer::Logger::LogMessage ("Failed to find field: %x",
                                                     hash);
                    return nullptr;
                }

            initialOffset += data->Structure->nBaseOffset;
            field = FindFieldData (data->Params, hash);
        }

    return reinterpret_cast<uint8_t *> (ptr) + field->nOffset + initialOffset;
}

/*******************************************************/
ParserEnumEquate<>
ParserUtils::FindFieldEnum (parStructureStaticData *data, void *ptr,
                            uint32_t hash)
{
    parMemberCommonData *field = FindFieldData (data->Params, hash);

    if (!field)
        throw std::runtime_error ("Failed to find enum field for parser"
                                  + std::to_string (hash));

    if (field->eType != parMemberType::ENUM)
        throw std::runtime_error ("Enum field for parser is not of enum type"
                                  + std::to_string (hash));

    parMemberEnumData *enumField = static_cast<parMemberEnumData *> (field);

    return ParserEnumEquate<> (*enumField->m_pTranslationTable,
                               FindFieldPtr (data, ptr, hash));
}

/*******************************************************/
ParserBitset
ParserUtils::FindFieldBitset (parStructureStaticData *data, void *ptr,
                              uint32_t hash)
{
    parMemberCommonData *field = FindFieldData (data->Params, hash);

    if (!field)
        throw std::runtime_error ("Failed to find bitset field for parser"
                                  + std::to_string (hash));

    if (field->eType != parMemberType::BITSET)
        throw std::runtime_error (
            "Bitset field for parser is not of bitset type"
            + std::to_string (hash));

    parMemberBitsetData *enumField = static_cast<parMemberBitsetData *> (field);

    return ParserBitset (*enumField->m_pTranslationTable,
                         FindFieldPtr (data, ptr, hash));
}

parStructureStaticData *
ParserUtils::FindStaticDataFromMemory (uint32_t hash)
{
    parStructureStaticData *out = nullptr;

    uint8_t *hashBytes      = reinterpret_cast<uint8_t *> (&hash);
    char     patternStr[16] = {0};
    sprintf (patternStr, "%02x %02x %02x %02x", hashBytes[0], hashBytes[1],
             hashBytes[2], hashBytes[3]);

    hook::pattern p (patternStr);
    if (p.size () == 1)
        {
            out                  = p.get (0).get<parStructureStaticData> ();
            m_pStaticDatas[hash] = out;

            return out;
        }
    else if (p.size () != 0)
        assert (!"Ambiguous pattern results for parser static data");

    throw std::runtime_error ("Unable to find parser static data");
}
