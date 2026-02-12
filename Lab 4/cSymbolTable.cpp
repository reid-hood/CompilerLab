// cSymbolTable.cpp - implementation of nested symbol table
#include "cSymbolTable.h"
#include "cSymbol.h"

// Define the global symbol table
cSymbolTable g_symbolTable;

cSymbolTable::cSymbolTable()
{
    // start with one global scope
    m_scopes.emplace_back();
}

symbolTable_t *cSymbolTable::IncreaseScope()
{
    m_scopes.emplace_back();
    return &m_scopes.back();
}

symbolTable_t *cSymbolTable::DecreaseScope()
{
    if (!m_scopes.empty()) m_scopes.pop_back();

    if (m_scopes.empty()) {
        // ensure there is at least one scope
        m_scopes.emplace_back();
    }

    return &m_scopes.back();
}

void cSymbolTable::Insert(cSymbol *sym)
{
    if (m_scopes.empty()) m_scopes.emplace_back();
    m_scopes.back()[sym->GetName()] = sym;
}

cSymbol *cSymbolTable::Find(string name)
{
    for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) return found->second;
    }

    return nullptr;
}

cSymbol *cSymbolTable::FindLocal(string name)
{
    if (m_scopes.empty()) return nullptr;
    auto &scope = m_scopes.back();
    auto found = scope.find(name);
    if (found != scope.end()) return found->second;
    return nullptr;
}
