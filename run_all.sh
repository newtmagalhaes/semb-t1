#!/bin/bash
set -e

echo "=== Compilando Testes e Benchmark ==="
make clean
make all

echo ""
echo "=== Executando Testes de Validação ==="
make run-tests

echo ""
echo "=== Executando Benchmark (Otimizado) ==="
make run-bench

echo ""
echo "Concluído! 🎉"
