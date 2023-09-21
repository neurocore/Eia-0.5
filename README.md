# Eia-0.5
Chess engine. Work in progress

# Usage
- Built by any C++17 compiler
- Connect to UCI-compatible chess GUI as engine

# Features
- **UCI communication protocol**\
Widely used protocol that allows engine to communicate with a graphical interface

- **Black magics (by Volker Annuss)**\
Alternative way to cook and use magics for fast attacks discovery.\
The main achievement of this approach is in lesser attack table\
(just 88507 entries, that equal to ~780kb)
