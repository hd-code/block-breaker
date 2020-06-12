# Block-Breaker HD

Projekt für das Fach Grafische Datenverarbeitung 2 an der Fachhochschule Erfurt von Hannes Dröse.

Es ist ein klassischer Breakout-Clone. Geschrieben in C++ und HLSL mit dem YoshiX-Framework.

## Installation

Das Projekt funktioniert nur auf Windows mit Visual Studio 2019.

...

## Development

Dieses Projekt liegt auf 2 Git-Repos gleichzeitig. Einmal auf Github und zusätzlich noch auf dem Git-Server der FH Erfurt. Damit ein Push immer auf beide Repos erfolgt, müssen beide als remotes registriert sein.

```sh
# wenn noch nicht geschehen, Repo von Github clonen
git clone https://github.com/hd-code/block-breaker.git

# in das Verzeichnis des Projektes wechseln
cd block-breaker

# Git-Server der FH als remote hinzufügen
git remote set-url --add --push origin https://source.ai.fh-erfurt.de/ha9384dr/block-breaker.git

# Überprüfen ob es geklappt hat
git remote -v
# => es sollten nun beide Repos für den push-Befehl hinterlegt sein
```