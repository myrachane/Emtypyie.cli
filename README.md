# EMTYPYIE.CLI v2.2.0

> Run emtypyie projects from your terminal with ease — the future runtime engine.

## Quick install

```sh
# Windows (Winget)
winget install myrachane.emtypyie-cli

# Windows (Chocolatey)
choco install emtypyie-cli

# npm (all platforms)
npm install -g emtypyie-cli
```

## Commands

| Command | Description |
|---------|-------------|
| `/help` | Show help |
| `/list` | List available projects from CDN |
| `/get <project>` | Install a project |
| `/get gcc` | Auto-install GCC/G++ compiler |
| `/info <project>` | Show project details |
| `/flash <project>` | Re-download latest version |
| `/rm <project>` | Remove project |
| `/theme <name>` | Change color theme |
| `/bf` | System info screen (bakafetch) |
| `/docs <project>` | Open project docs |
| `/shell` | Interactive mode |
| `/clear` | Clear screen |

## Structure

| Path | Description |
|------|-------------|
| `root4c/`    | C CLI — portable single binary (primary) |
| `root4node/` | Node.js CLI — published to npm |
| `mainsite/`  | Website landing pages |
| `manifests/` | Winget package manifests |
| `choco/`     | Chocolatey package |

## C CLI (root4c)

Single-binary CLI written in C11, no runtime dependencies.

- **Build:** `cmake -B build && cmake --build build`
- **Themes:** slate, green, amber, violet, cyan
- **Bakafetch:** Unicode/braille art system info screen
- **CDN registry:** fetches project list and metadata from `cdn.emtypyie.in/dev`
- **No args:** opens interactive shell
