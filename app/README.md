# Executáveis do Projeto calabreso.txt

Este repositório contém duas versões de executáveis para rodar o projeto **calabreso.txt** sem necessidade de configuração adicional.

**Nota:** Para garantir simplicidade e execução em tempo real, os executáveis operam de forma otimizada e diferente do projeto original. Consulte a seção [Observações](#observações) para entender as diferenças.

---

## Executáveis Disponíveis

1. **Ubuntu**:

   - Desenvolvido para distribuições Ubuntu que utilizam versões antigas do OpenCV disponíveis no APT.
   - Nome do arquivo: `Ubuntu`

2. **Linux (Geral)**:

   - Compatível com distribuições Linux que utilizam a versão mais recente do OpenCV instalada manualmente.
   - Nome do arquivo: `Linux`

3. **Linux (Com detecção de movimento)**:

   - Versão com detecção de movimento, que destaca áreas em movimento no vídeo.
   - Nome do arquivo: `Linux_Movement`

   Obs.: Pode ser necessário baixar o pacote `ffmpeg-devel` para executar este arquivo.

---

## Requisitos

### 1. **Ubuntu e derivados (usando APT)**

```bash
 apt-get install libopencv-dev
```

Este executável é otimizado para trabalhar com os pacotes padrão do Ubuntu.

---

### 2. **Linux (Distribuições com OpenCV Atualizado)**

Para distribuições Linux com suporte a versões mais recentes do OpenCV, é necessário instalar o OpenCV manualmente:

Arch:

```bash
pacman -S opencv
```

Fedora:

```bash
dnf install opencv-devel
```

---

### 3. **Linux (Com Detecção de Movimento)**

Para executar a versão com detecção de movimento, é necessário instalar o pacote `ffmpeg-devel`:

Arch:

```bash
pacman -S ffmpeg
```

Fedora:

```bash
dnf install ffmpeg-devel
```

---

## Como Usar

Execute o comando desejado para iniciar o player:

```bash
./Ubuntu <caminho_para_o_video>
# ou
./Linux <caminho_para_o_video>
# ou
./Linux_Movement <caminho_para_o_video>
```

O vídeo será ajustado automaticamente ao tamanho do terminal e reproduzido diretamente como um player de vídeo ASCII.

Se preciso, dê permissão de execução ao arquivo:

```bash
chmod +x Ubuntu
# ou
chmod +x Linux
# ou
chmod +x Linux_Movement
```

---

## Dicas para Melhor Qualidade

- **Fonte Menor**: Reduza o tamanho da fonte do terminal para obter maior definição nos caracteres.
- **Janela Grande**: Maximize a janela do terminal para permitir maior espaço para o vídeo.

---

## Observações

**Este modo de execução utiliza uma técnica baseada em **brightness** para determinar os caracteres do vídeo, o que pode resultar em uma resolução menor comparada ao modo completo disponível no projeto original.**

Para experimentar a versão completa, que permite configurações detalhadas e melhores resultados visuais, consulte o README original [aqui](../README.md).

Para outras versões Linux, você pode compilar o arquivo `LINUX.cpp` manualmente.

Uma versão para Windows está disponível [aqui](WIN.cpp), porém o executável não está disponível neste repositório. Para utilizar, compile o arquivo `Windows.cpp` manualmente.

---

Obrigado novamente pelo suporte! :smile:
