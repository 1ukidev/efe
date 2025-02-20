#pragma once

#include "efe/DAO.hpp"
#include "efe/configuracao/entities/UsuarioEntity.hpp"

namespace efe::configuracao
{
    class UsuarioDAO final : public DAO<UsuarioEntity>
    {
    public:
        UsuarioDAO() = default;
        ~UsuarioDAO() = default;
    };
}
