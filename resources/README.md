# Resources

## `html`
Nesta pasta é necessário que os arquivos `.html` sejam convertidos em `.hpp` para uso posterior no servidor.

Para isso podemos utilizar a ferramenta `xxd`.

Exemplo:
```bash
xxd -i index.html > index.hpp
```
