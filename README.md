# 🐚 minishell

## Description

**minishell** est une réimplémentation d’un shell minimaliste en C.  
Il permet d’exécuter des commandes, de gérer les **pipes**, les **redirections**, et de manipuler les **variables d’environnement**.  

Ce projet a pour objectif de comprendre en profondeur le fonctionnement d’un shell Unix et la gestion des processus.

---

## 🎯 Objectifs du projet

- Exécuter des commandes simples et complexes
- Gérer les **pipes** (`|`) et les **redirections** (`>`, `>>`, `<`)
- Implémenter les **built-ins** :
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Gérer les **variables d’environnement**
- Implémenter une **boucle principale** (prompt interactif)
- Respecter la **norme 42**

---

## 🧠 Fonctionnalités

- Exécution de commandes simples ou avec arguments
- Gestion des **pipes multiples**
- Redirections d’entrée et de sortie
- Expansion des variables d’environnement (`$PATH`, `$HOME`, etc.)
- Gestion des **quotes** simples et doubles
- Gestion des erreurs avec messages explicites
- Signal handling (`Ctrl+C`, `Ctrl+D`)

---

## ⚙️ Compilation

```bash
make
