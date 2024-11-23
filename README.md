**Oi TikTok! Estou preparando executáveis pro projeto rodar direto (sem qualquer configuração necessária) tanto no Windows quanto no Linux. Vou deixar o vídeo do Calabreso por padrão também!**

Enquanto isso, as instruções pra rodar estão logo abaixo (só funciona no Linux)

**Obrigado pelo suporte 🌠:**

# calabreso.txt

Esse é um projeto que transforma um vídeo em arte ASCII, convertendo cada quadro em texto usando qualquer fonte que você quiser. Ele oferece dois modos de saída:

1. Reproduz o vídeo em formato de texto diretamente no terminal.
2. Salva os quadros em ASCII como um arquivo `.mp4` renderizado na pasta `output`.

**Exemplo** (leva alguns segundos para carregar):
![Exemplo de Vídeo](public/sampletxt.gif)

---

## Requisitos

- `make` (GNU)
- OpenCV2 (pode instalar com `libopencv-dev` no Ubuntu ou `opencv` no Fedora e Arch)
- Compilador g++ (para o motor em C++)
- Python3 (para o motor em Python)

---

## Configuração (sistemas UNIX)

1. Clone o repositório:

```bash
  git clone https://github.com/yantavares/video.txt.git
  cd video.txt
```

2. Se precisar, torne o arquivo `play.sh` executável:

```bash
  chmod +x play.sh
```

3. Instale os pacotes necessários:

```bash
   # Para Ubuntu
   sudo apt-get install libopencv-dev
   pip install -r requirements.txt
```

4. **(Opcional)** Adicione seu arquivo de vídeo na pasta `videos`, ou use o `SampleVideo.mp4` já fornecido.

5. **(Opcional)** Coloque sua fonte preferida (.ttf) na pasta `fonts`, ou use a fonte padrão `ComicMono.ttf`.

---

## Como rodar o projeto

Para rodar o projeto, use o comando `make`:

```bash
   make
```

Você vai escolher entre dois modos de execução:

1. **Reproduzir no terminal**: Mostra o vídeo em ASCII direto no seu terminal.
2. **Salvar como MP4**: Gera um `.mp4` renderizado na pasta `output`.

Depois de escolher o modo, siga os passos descritos abaixo.

---

### Passos após rodar o `make`:

1. **Selecionar a fonte**:
   - A padrão é `Comic Mono`.
2. **Escolher o tamanho da fonte**:
   - **Deve ser compatível com o tamanho da fonte do terminal**.
   - Escolha entre 2 e 20.
   - O padrão é 11.
3. **Informar o nome do vídeo**:
   - Escolha um vídeo da pasta `videos`.
   - O padrão é `SampleVideo.mp4`.
4. **Selecionar o modo**:
   - Escolha uma das opções:
     - **1**: Reproduzir no terminal.
     - **2**: Salvar como `.mp4` na pasta `output`.

Se você não responder algum dos prompts, os valores padrão serão usados.

---

### Comandos

- Para **reproduzir o vídeo no terminal** após rodar `make`:

```bash
   make play
```

O vídeo será redimensionado automaticamente para caber na janela do terminal.

- Para **salvar o vídeo renderizado em ASCII como um MP4**:

```bash
   make
```

Escolha o modo `2` quando solicitado, e o arquivo será salvo como `output/text.mp4`.

---

### Saídas

1. Se você escolher o **Modo 1**, o vídeo será exibido diretamente no terminal.
2. Se você escolher o **Modo 2**, o vídeo renderizado em ASCII será salvo como `.mp4` na pasta `output`.

Além disso, todos os quadros individuais do vídeo em ASCII serão salvos como arquivos `.txt` na pasta `output`.

---

## Licença

Esse projeto está sob a licença MIT.

---

## Contribuições

Contribuições são super bem-vindas! Abra uma issue ou envie um pull request para qualquer melhoria ou sugestão. 😄
