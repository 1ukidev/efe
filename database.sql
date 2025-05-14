-- Script para criar o banco de dados e as tabelas necessárias.

CREATE DATABASE efe;

CREATE TABLE IF NOT EXISTS usuario (
    id BIGSERIAL PRIMARY KEY,
    nome TEXT NOT NULL,
    login TEXT NOT NULL UNIQUE,
    senha TEXT NOT NULL
);

-- Senha: root
INSERT INTO usuario (nome, login, senha)
VALUES ('Administrador', 'root', '$2a$12$K2WoajnbIlwBnXM9Wh1kOuM7poeOwSEI4jkzzjLGS8sIVPr2sw0hu');
