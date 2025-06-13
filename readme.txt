# TermiRunner

A **minimal** 60 FPS endless runner that runs entirely in your UNIX terminal.
Written in ISO C11 with **ncurses**, a timer signal tick loop, and a forked input handler.

---

## Quick Build & Run

```bash
./run.sh            # builds with make and launches the game
```

The script should work on most **Linux** distributions out‑of‑the‑box.
For **macOS**, install `ncurses` via Homebrew (`brew install ncurses`) and run the same command.

### Requirements

* gcc or clang + make
* ncurses (Linux: `libncursesw`, macOS: Homebrew `ncurses`)

## Controls

| Key                | Action |
| ------------------ | ------ |
| **SPACE** or **↑** | Jump   |
| **q**              | Quit   |

## Screen Size

Designed for 80 × 24 terminals or wider. Wider terminals simply show more ground ahead.

---

This is an early scaffold; gameplay tuning, scoring, and additional visuals will be added later.

