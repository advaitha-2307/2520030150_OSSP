# OSSP Skill 6

## Topic

Process Escape Sequences and Child Process Execution

## Objectives

### Part 1: Process Escape Sequences

- Handle escaped spaces
- Escape special symbols
- Preserve escaped characters
- Validate parser output
- Test complex inputs

### Part 2: Child Processes

- Create child processes using `fork()`
- Execute programs using `execvp()`
- Handle execution errors
- Pass arguments to programs
- Manage the parent process using `waitpid()`
- Test command launching

## Project Structure

```text
Skill_6/
├── Makefile
├── README.md
├── bin/
│   ├── escape_parser
│   └── execute_command
└── src/
    ├── escape_parser.c
    └── execute_command.c
