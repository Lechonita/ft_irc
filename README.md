# IRC - Internet Relay Chat


## 1. En tant que dev, je veux pouvoir accueillir plusieurs utilisateurs dans mon serveur

	- La communication entre client et serveur se fait par TCP/IP (v4 ou v6)

	- Je peux avoir autant d’utilisateurs que je veux

	- Je veux pouvoir les différencier entre utilisateurs regular et les opérateurs

	- Un de mes utilisateurs sera mon utilisateur référent qui sera utilisé en évaluation

	- Mes utilisateurs pourront utiliser le mdp du serveur pour le rejoindre



## 2. En tant que dev, je veux pouvoir ajouter des opérations I/O

	- Utiliser seulement 1 seul poll() (ou équivalent) pour toutes ces opérations



## 3. En tant qu’utilisateur, je veux pouvoir me connecter au serveur

	- Je veux pouvoir me connecter au serveur:

	- Utilisation du login

	- Utilisation du mdp

	- Message d’erreur approprié si le mdp est invalide

	- Rajouter un pseudo/changer ?

	- Rejondre un channel (est-ce qu’il y a plusieurs channels dans un serveur ? ou bien un serveur est un channel ?)



## 4. En tant qu’utilisateur, je veux pouvoir envoyer et recevoir des messages

	- Tous les messages d’un utiisateur doivent être envoyés à tous les utilisateurs du serveur/channel



## 5. En tant qu’utilisateur opérateur, je veux pouvoir faire plusieurs actions dans le serveur

	- KICK

	- INVITE

	- TOPIC

	- MODE
		* i

		* t

		* k

		* o

		* l