#!/bin/bash
#SBATCH --job-name=vadd_omp      
#SBATCH --partition=amd-512     
#SBATCH --time=0-0:1            

# --- Configuração de Recursos para OpenMP ---
#SBATCH --nodes=1               
#SBATCH --ntasks=1               
#SBATCH --cpus-per-task=64     

echo "Job OpenMP iniciado em: $(date)"

# Define o número de threads OpenMP para ser igual ao número de CPUs que o SLURM alocou
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

echo "Executando com $OMP_NUM_THREADS threads..."

# --- Programa a ser executado ---
# Não usamos mpirun ou srun para programas OpenMP simples
./vadd_par

echo "Job concluído em: $(date)"
