## Termux Launch

- Requires x11-repo and termux-x11 installed (both pkg and app)
- Requires `jq` when building to generate `./compile_commands.json` file for lsp resolution of SDL

- Might also requires zenity to be installed (confirm functionality with the dialog script)

## Prep, Build and Launch

This will fore up tx11, update env vars and launch the program

```bash
source ./scripts/termux11.sh
./build.sh
./out/handmade
```

Do not combine the two commands, x11 should only be started once per session and done via source.  You may have to restart termux fully in case of a mistake

## Debug

In nvim open a new tab with `<leader>tt`

```bash
source ./scripts/termux11.sh
./build.sh
```

Fire up debugger in a cpp file with `<leader>dd`.

This should automatically launch with the handmade binary.
