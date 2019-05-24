# Features 

## Done
- Bouton START
- Délai de 3 secondes avant début du décompte (Constante dans le code)
- Compte à rebours de 60 secondes (Constante dans le code)
- Led à fréquence variable en fonction du temps restant
- Confirmation visuelle de l'explosion : augmentation de la luminosité + glignotement de "8888" sur l'afficheur
- Confirmation visuelle du désamorcage : diminution de la luminosité + arrêt du décompte
- Cycle de vie (STARTING, PLAYING, WINNING, LOOSING, ENDING)
- Triggers affectés de manière aléatoire

## TODO
- Beep de stress pour accompagner la Led clignotante
- Ruban RGB 

# Cablage
- 2 plaques: 1 Read Only et 1 à désamorcer
- Les cables gris sont entortillés / à ne pas débrancher (ie: il passe sous la mousse)
- Les cables à débrancher sur la plaque du haut sont en couleurs.
- Il suffit de faire un pont entre les lignes impaires, peu importe le numéro

![schema](Stunning%20Inari-Hango.png)

