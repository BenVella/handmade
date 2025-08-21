## Termux Launch

- Requires x11-repo and termux-x11 installed (both pkg and app)

Run `scripts/termux11.sh` **as source**:

```bash
source ./scripts/termux11.sh
```

Wait a moment for the x11 app to Launch then run the compiled binary

```bash
./out/handmade
```

Do not combine the two commands, x11 should only be started once per appropriate session and done via source.  You may have to restart termux fully in case of a mistake
