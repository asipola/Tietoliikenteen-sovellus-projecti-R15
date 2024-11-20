# Tietoliikenteen sovellus projecti R15

Tämä on koulu projekti, jossa kerätään kiihtyvyysanturin dataa ja lähetetään sitä bluetoothin yli nordic embedded laitteella. Seuraavaksi raspberry pi ottaa bluetooth signaalin kiinni ja tallentaa saadun datan tietokantaan. Tietokannan data tallennetaan csv tiedostoksi, jota käytetään algorytmiä laskeaksemme eri suuntien keskipisteet. Nyt kun keskipisteet ovat tiedossa käytämme niitä kiihtyvyysanturin koodissa, jotta tiedämme suoraan mihin suuntaan anturi osoittaa.

<img src="images/arkkitehtuuri kaavio.PNG">

Confusion matriisi jolla näemme algorytmin tarkkuuden, testissämme se tuotti sadan prosentin osuvuuden.
<br> Kolumit ovat algorytmin laskema suunta, joka otettu kiihtyvyys anturista ja rivit ovat käyttäjän kertomat suunnat.<br>
<img src="images/matriisi.png">
![Posteri R15](https://github.com/user-attachments/assets/d2f0c746-a0f5-4937-aa93-10c23ebcc0d6)
