# Texture wechseln mit flag/enum???

- mit yoshix eher schwierig
- ein material pro textur

im pixel constant buffer => uint TextureIndex
switch case anweisung für die Textur

# Constant Buffer PS, für Licht-Informationen? Direktional vs Punktlicht

# Wann upload Licht? schon verfügbar on Startup (Constant Buffers kommen später) onResize?

InternOnStartup für statische Lichtquelle und co sollte funktionieren
Geht auch on Frame ... wäre auch okay ...

# Specular Exponent? Einfach ein Genereller ? oder per Objekt

um die 80...100 rum
sonst eigentlich über constant buffer