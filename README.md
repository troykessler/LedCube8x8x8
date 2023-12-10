# LedCube8x8x8

## Login LedCube

`Login:`

Mit ssh einloggen

```bat
ssh -i ~/.ssh/raspberry troykessler@192.168.1.x
```

`Files hochladen:`

Mit scp hochladen

```bat
scp -i ~/.ssh/raspberry ledbuce.c pi@192.168.1.x:
```

```bat
sudo apt-get update
sudo apt-get install pigpio
```

`ledcube.c kompilieren:`

```bat
gcc -o ledcubeExe ledcube.c -lm -lrt -pthread -lpigpio
```

## LedCube Software Animationen

`Animationen Übersicht:`

1.  Ebenen, Schichten werden schnell und zufällig geswitcht
2.  Punkte werden generiert und wieder gelöscht, Gesamtzahl bleibt im Gleichgewicht
3.  Regen Animationen
4.  Es existieren zwei Schichten, wobei einzelne Punkte schnell die Position wechseln
5.  Würfel Rahmen zoomen zufällig von Ecke zu Ecke
6.  Das Zentrum eines dreidemensionalen Sinusfunktion wird gezeigt
7.  Genau wie 6., allerdings rotiert das Zentrum außerhalb des Sichtspektrums
8.  Feuerwerksanimation

`Animationsideen:`

- Buchstaben, Zahlen -> Nachrichten anzeigen
