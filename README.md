# Tsock

Tsock est un programme qui permet l'envoi et la reception de messages entre différents hôtes qu'ils se trouvent sur une même machine ou pas.

## Compilation

Pour compiler le projet, ouvrez un terminal dans ce directory et entrez :

```bash
make
```

## Utilisation

### Fonctionnement SOURCE :
```bash
./tsockc -s [destinataire][port]
```

### Fonctionnement PUITS :
```bash
./tsockc -s [port]
```

### Options SOURCE et PUITS :
 - Fonctionnement UDP : [-u] (par défaut TCP)
 - Nombre de messages : [-n##]
 - Longueur des messages : [-l##]

### Fonctionnement SERVEUR BAL
```bash
./tsockc -b [port]
```

### Fonctionnement EMMETTEUR BAL
```bash
./tsockc -e[##] [destinataire][port]
```

### Fonctionnement EMMETTEUR BAL
```bash
./tsockc -r[##] [destinataire][port]
```
### Options BAL :
 - Nombre de messages : [-n##]
 - Longueur des messages : [-l##]

## GitHub

Toutes les commentaires sont les bienvenus.
Projet complet et mises à jour éventuelles du programme sur GitHub : https://github.com/christalecrouard/tsock

## Commentaires et explication
Je n'ai pas eu le temps de gérér des messages de taille différentes dans le même BAL, ni la soustraitance de la BAL (avec fork et select).
On peut en observer une ébauche dans ./bal.c/boite2
J'ai oublié la plus part des free...


## License
[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)



