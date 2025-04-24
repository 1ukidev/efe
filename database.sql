-- Script para criar o banco de dados e as tabelas necessárias.

CREATE DATABASE efe;

\c efe

CREATE TABLE IF NOT EXISTS usuario (
    id BIGSERIAL PRIMARY KEY,
    nome TEXT NOT NULL,
    senha TEXT NOT NULL
);
