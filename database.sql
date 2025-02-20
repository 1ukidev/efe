-- Script para criar o banco de dados e as tabelas necessárias.

CREATE DATABASE efe;

\c efe

CREATE TABLE IF NOT EXISTS usuario (
    id BIGSERIAL PRIMARY KEY,
    nome VARCHAR(255) NOT NULL,
    senha VARCHAR(255) NOT NULL
);
