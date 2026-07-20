# Emtypyie.cli@v2.5.8 Stable

> Run emtypyie projects from your terminal with ease — the future runtime engine.

## Quick install

```sh
# Windows (Winget) — C native build
winget install myrachane.emtypyie-cli

# Windows (Chocolatey) — C native build
choco install emtypyie-cli

# npm (all platforms) — Node.js version
npm install -g emtypyie-cli
```

Two binary builds available: **C native** (self-contained, no runtime deps) for Windows, and **Node.js** (ported with pkg) for multi-platform.

## Launch

Once installed, the binary is on your `PATH` as `emtypyie`. Just type it in **Command Prompt, PowerShell, or Windows Terminal**:

```sh
emtypyie
```

This prints the startup boot animation and drops you into the **interactive shell**.
Type `/help` for commands, or `/about` for version info.

To skip the boot animation:

```sh
emtypyie --no-animation
# or
set EMTYPYIE_NO_ANIM=1
```

Direct commands also work without entering the shell, e.g. `emtypyie /list` or `emtypyie /get gcc`.

## Commands

| Command | Description |
|---------|-------------|
| `/help` | Show help |
| `/list` | List available projects from CDN |
| `/get <project>` | Install a project |
| `/get gcc` | Auto-install GCC/G++ compiler |
| `/get larpino@1b` | Download a LLAMA GGUF model and load it |
| `/info <project>` | Show project details |
| `/flash <project>` | Re-download latest version |
| `/rm <project>` | Remove project |
| `/theme <name>` | Change color theme |
| `/bf` | System info screen (bakafetch) |
| `/docs <project>` | Open project docs |
| `/shell` | Interactive mode |
| `/larpino enable\|disable\|status` | LLAMA inference engine |
| `/clear` | Clear screen |

## Structure

| Path | Description |
|------|-------------|
| `archive/vX.Y.Z/Root4c/`    | C CLI — portable single binary (primary, recommended) |
| `archive/vX.Y.Z/Root4node/` | Node.js CLI — published to npm |
| `mainsite/`  | Website landing pages |
| `manifests/` | Winget package manifests |
| `choco/`     | Chocolatey package |

## C CLI (Root4c)

Single-binary CLI written in C11/C++17, no runtime dependencies.

- **Build:** `cmake -B build && cmake --build build` (MinGW-w64 / MSVC)
- **Themes:** slate, green, amber, violet, cyan
- **Bakafetch:** Unicode/braille art system info screen
- **Larpino:** Built-in LLM inference engine
  - Loads GGUF format LLAMA models (Q4_0, Q4_1, Q5_0, Q5_1, Q8_0, F16, F32)
  - BPE tokenizer with merge rules
  - KV-cached transformer (RoPE, SwiGLU, RMS norm, GQA)
  - Top-k / temperature sampling
  - `/larpino enable` enters chat mode in the interactive shell
  - `/get larpino@1b` downloads a model from the CDN
- **CDN registry:** fetches project list and metadata from `cdn.emtypyie.in/dev`
- **No args:** opens interactive shell (with startup boot animation)

## Release artifacts

Each GitHub release ships two Windows executables:

| ZIP | Contents | Source |
|-----|----------|--------|
| `emtypyie-cli-windows-x64-2.5.8.zip` | `emtypyie.exe` | Node.js (pkg) |
| `emtypyie-cli-native-windows-x64-2.5.8.zip` | `emtypyie.exe` | C native build |
