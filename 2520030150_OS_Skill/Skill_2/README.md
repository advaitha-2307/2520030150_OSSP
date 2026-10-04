# OSSP Skill 2

## Objective

To create an interactive shell loop that displays a prompt,
reads user input, handles exit conditions, captures keyboard
input, handles Backspace and Enter, manages an input buffer,
and supports multi-character commands.

## Features

- Main interactive loop
- Display shell prompt
- Read user input
- Exit condition
- Keyboard input
- Backspace handling
- Enter key handling
- Input buffer
- Multi-character commands
- Empty input handling

## Control Flow

```text
START
  |
  v
Display Prompt
  |
  v
Capture Keyboard Input
  |
  v
Store Character in Buffer
  |
  v
Check Enter / Backspace
  |
  v
Complete Input
  |
  v
Check for "exit"
  |
  +---- YES ----> EXIT
  |
  NO
  |
  v
Display Received Command
  |
  v
Return to Prompt
