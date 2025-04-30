-- Script para criar o banco de dados e as tabelas necessárias.

create database efe;

\c efe

create table if not exists usuario (
    id bigserial primary key,
    nome text not null,
    login text not null unique,
    senha text not null
);

-- Senha: root
insert into usuario (nome, login, senha)
values ('Administrador', 'root', '$2a$12$K2WoajnbIlwBnXM9Wh1kOuM7poeOwSEI4jkzzjLGS8sIVPr2sw0hu');
