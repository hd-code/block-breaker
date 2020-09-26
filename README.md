# BlockBreaker HD

Dies ist das Projekt für das Fach GDV2 an der FH Erfurt von Hannes Dröse.

Es ist ein klassischer Breakout-Clone. Geschrieben in C++ und HLSL mit dem YoshiX-Framework.

## Installation

Es muss lediglich das Git-Repository geklont werden. Danach lässt sich das Projekt mit Visual Studio öffnen und starten.

```sh
git clone https://github.com/hd-code/block-breaker.git
```

Das Projekt kann nur auf Windows mit Visual Studio gebaut und gestartet werden.

Das VS-Projekt befindet sich in: `project/project.sln`.

## Controls

Das Spiel kann mit der `Leertaste` pausiert werden. Ebenso wird es damit fortgesetzt. Sobald ein Spiel gewonnen oder verloren ist, kann es ebenfalld mit der `Leertaste` erneut gespielt werden.

Die Steuerung des Paddles erfolgt mit der `linken` bzw. `rechten Pfeiltaste`.

## Development

Dieses Projekt liegt auf 2 Git-Repos gleichzeitig. Einmal auf Github und zusätzlich noch auf dem Git-Server der FH Erfurt. Damit ein Push immer auf beide Repos erfolgt, müssen beide als remotes registriert sein.

```sh
# wenn noch nicht geschehen, Repo von Github clonen
git clone https://github.com/hd-code/block-breaker.git

# in das Verzeichnis des Projektes wechseln
cd block-breaker

# beide Git-Server als remote hinzufügen
git remote set-url --add --push origin https://github.com/hd-code/block-breaker.git
git remote set-url --add --push origin https://source.ai.fh-erfurt.de/ha9384dr/block-breaker.git

# Überprüfen ob es geklappt hat
git remote -v
# => es sollten nun beide Repos für den push-Befehl hinterlegt sein
```