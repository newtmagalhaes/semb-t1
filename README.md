# ChaCha20 - Implementação em C (Embarcados)

Implementação leve, eficiente e em conformidade com operações bit a bit do algoritmo de criptografia **ChaCha20**, projetada para portabilidade e validação com **sistemas embarcados**.

Código desenvolvido sem uso de alocação dinâmica e validado obrigatoriamente através dos vetores oficiais da documentação **RFC 8439**.

## 📊 Estrutura do Projeto

* `src/`: Lógica central do algoritmo (`chacha20.c` e `.h`). Organiza matriz de estado e quarter rounds. O módulo `chacha20_mt.c/h` expande o algoritmo base usando a biblioteca `pthreads`.
* `tests/`: Bateria de validação.
  * `test_rfc.c`: Validando dados byte a byte usando test vectors pré definidos.
  * `test_8kb.c`: Garantindo o comportamento iterativo cifrando e decifrando buffers inteiros de 8192 bytes.
  * `test_mt.c`: Garante corritude bit-a-bit das saídas multi-thread comparadas às saídas Single Thread de base usando particionamento de tarefas e arrays estáticos parciais.
* `bench/`: Ferramenta para medição e stress. Retorna estatisticamente custo por bloco (ns) e processamento gerado em MB/s.
  * O utilitário `bench_mt` instancia dinamicamente o número de núcleos (`sysconf(_SC_NPROCESSORS_ONLN)`) disponíveis da máquina corrente, fazendo comparações automáticas de escala processando bufferes massivos na ordem de MegaBytes (ex. 8MB estáticos).

## 🚀 Como Executar

### Utilizando o Script Direto (Recomendado)
Para compilar e rodar toda a suíte automaticamente sem complicação:

```bash
chmod +x run_all.sh
./run_all.sh
```

### Utilizando Makefile Manualmente
A automação com `Make` permite o tratamento individual das instâncias:

```bash
# Compila Testes e Benchmark
make all

# Roda os Testes Funcionais
make run-tests

# Roda Bateria de Benchmarks
make run-bench

# Limpa Binários
make clean
```

## 🛠️ Requisitos Técnicos 
- Manipulação total dentro de limite local estático `uint8_t data[8192]`. Sem uso de `malloc`, `alloc` e derivados.
- Endianness mantido como **little-endian**.
- Medições do benchmark realizam saltos que evitam gargalos de Input e Output.
