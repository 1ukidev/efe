```
       __
  ___ / _| ___
 / _ \ |_ / _ \
|  __/  _|  __/
 \___|_|  \___|
```

O efe é uma base para APIs aonde o desempenho é crucial. É feito em C++ moderno com a ajuda de projetos como o [Drogon](https://github.com/drogonframework/drogon) e [TRANTOR](https://github.com/an-tao/trantor), além de ser pensado de forma que a manutenção não seja dolorosa, ou seja, nada de funções assíncronas aninhadas ou abstrações complexas. Se aproveita do multithreading e das corrotinas para conseguir lidar com milhares de solicitações por segundo e minimizar a latência.

Veja mais aqui: [1ukidev.github.io/postagens/api-cpp](http://1ukidev.github.io/postagens/api-cpp)

## TODO
- Testes unitários
- clang-format

## Algumas bibliotecas utilizadas
- [Drogon](https://github.com/1ukidev/drogon)
  - [TRANTOR](https://github.com/an-tao/trantor)
  - [Brotli](https://github.com/google/brotli)
  - [libpq](https://github.com/postgres/postgres)
  - [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
  - [spdlog](https://github.com/gabime/spdlog)
- [Boost](https://github.com/boostorg/boost)
  - [program_options](https://github.com/boostorg/program_options)
- [jwt-cpp](https://github.com/Thalhammer/jwt-cpp)
- [toml++](https://github.com/marzer/tomlplusplus)
- [libbcrypt](https://github.com/1ukidev/libbcrypt)
