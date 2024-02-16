# [IRC](https://datatracker.ietf.org/doc/html/rfc1459) - Internet Relay Chat


## 1. En tant que dev, je veux pouvoir créer un serveur

- Créer un [serveur](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
	* man [socket](https://man7.org/linux/man-pages/man2/socket.2.html)


## 2. En tant que dev, je veux pouvoir accueillir plusieurs utilisateurs dans mon serveur

- La communication entre client et serveur se fait par [TCP/IP](https://www.fortinet.com/fr/resources/cyberglossary/tcp-ip) (v4 ou v6)

- Je peux avoir autant d’utilisateurs que je veux

- Je veux pouvoir les différencier entre utilisateurs regular et les opérateurs

- Un de mes utilisateurs sera mon utilisateur référent qui sera utilisé en évaluation

- Mes utilisateurs pourront utiliser le mdp du serveur pour le rejoindre

- Il faut utiliser IRSSI comme client (attention à la syntaxe)



## 3. En tant que dev, je veux pouvoir ajouter des opérations I/O

- Utiliser seulement 1 seul poll() (ou équivalent) pour toutes ces opérations
	* [I/O operations](https://www.techtarget.com/whatis/definition/input-output-I-O) 



## 4. En tant qu’utilisateur, je veux pouvoir me connecter au serveur

- Je veux pouvoir me connecter au serveur:

- Utilisation du login

- Utilisation du mdp

- Rajouter un pseudo unique (nickname)

- Message d’erreur approprié si le mdp est invalide

- Rejondre un channel



## 5. En tant qu’utilisateur, je veux pouvoir envoyer et recevoir des messages

- Tous les messages d’un utiisateur doivent être envoyés à tous les utilisateurs du serveur/channel



## 6. En tant qu’utilisateur opérateur, je veux pouvoir faire plusieurs actions dans le serveur

- KICK

- INVITE

- TOPIC

- MODE
	* i
	* t
	* k
	* o
	* l


--------------------------------------------------------------------------

# RÈGLES DU CLEAN CODE


## Fonctions

- 1 fonction = 1 action (faire une fonction pour tout !)
- Un verbe par fonction >> is_valid, display_message, compute_formula...
- Utiliser boolean pour les états binaires >> is_whitespace, is_digit...


## Magic Numbers

- Pas de magic numbers !
- Utiliser des defines autant que possible:
	* defines existants >> STDOUT_FILENO, EXIT_SUCCESS...
	* defines personnalisés >> ARG_COUNT, ERROR...


## Autres réflexes

- Utiliser des size_t pour les itérateurs >> size_t i = 0
- Utiliser ++i au lieux de i++ (when applicable)
- Utiliser des const !
- Utiliser des static

## Rendre le code lisible

- Lisibilité > optimisé
	* exemple: !isFalse()  vs  isFalse() == false