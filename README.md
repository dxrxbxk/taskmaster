# 🚀 📡 Taskmaster

Taskmaster is a process manager inspired by [Supervisor](http://supervisord.org/), developed as part of a project at 42 school. It allows efficient control and supervision of processes through a centralized configuration system.

## 📋 Key Features

- **Process Management:** Start, stop, and restart processes with simple commands.
- **Flexible Configuration:** Use a configuration file to define process behavior (restarts, logging, etc.).
- **Process Monitoring:** Retrieve real-time information about supervised processes.
- **Remote Control:** `taskcontrol` client interface to interact with `taskmaster` remotely.

## 🧬 Project Structure

- **Taskmaster:** The main server managing the processes.
- **Taskcontrol:** A client interface to control `taskmaster` from a remote terminal.

## ⚙️ Installation

1. Clone the repository:
   ```zsh
   git clone 'https://github.com/123Untitled/taskmaster.git'
   cd taskmaster
   ```
