Nume: Visanescu Bogdan-Emilian  
Grupa: 323CA

# PCOM Tema 3 - Client HTTP

Acest proiect reprezinta un client C++ pentru interactiunea cu un server REST, permitand gestionarea utilizatorilor, filmelor si colectiilor de filme. Aplicatia comunica cu serverul folosind protocoale HTTP si date in format JSON.

> **Nota:** API-ul de comunicare socket/HTTP implementat in `helper.cpp` este partial preluat si adaptat din materialele de laborator PCOM.

---

## Structura proiectului

- **client.cpp**  
  Fisierul principal al aplicatiei. Contine functia `main()` si toate functiile pentru interactiunea cu serverul: autentificare, gestionare utilizatori, filme si colectii.  
- **client.hpp**  
  Contine macrouri pentru URL-uri, headere HTTP si utilitare pentru a simplifica codul din `client.cpp`.
- **helper.hpp / helper.cpp**  
  Functii ajutatoare pentru constructia si parsarea requesturilor si raspunsurilor HTTP, precum si pentru comunicarea cu serverul prin socket.
- **nlohmann/json.hpp**  
  Biblioteca externa pentru manipularea obiectelor JSON in C++. Este folosita pentru a construi si interpreta datele trimise/primite de la server.

---

## Functionalitati detaliate

Aplicatia permite urmatoarele operatii, fiecare fiind accesibila printr-o comanda introdusa de la tastatura:

### Administrare utilizatori (admin)
- **login_admin** – Autentifica un administrator.
- **add_user** – Adauga un utilizator nou (doar ca admin).
- **get_users** – Listeaza toti utilizatorii existenti.
- **delete_user** – Sterge un utilizator dupa username.
- **logout_admin** – Delogheaza administratorul curent.

### Utilizator obisnuit
- **login** – Autentifica un utilizator.
- **logout** – Delogheaza utilizatorul curent.
- **get_access** – Obtine token de acces la biblioteca de filme.

### Filme
- **add_movie** – Adauga un film nou.
- **get_movies** – Listeaza toate filmele disponibile.
- **get_movie** – Afiseaza detalii despre un film dupa ID.
- **delete_movie** – Sterge un film dupa ID.
- **update_movie** – Actualizeaza detaliile unui film.

### Colectii de filme
- **add_collection** – Creeaza o colectie noua de filme.
- **get_collections** – Listeaza toate colectiile disponibile.
- **get_collection** – Afiseaza detalii despre o colectie dupa ID.
- **add_movie_to_collection** – Adauga un film intr-o colectie.
- **delete_collection** – Sterge o colectie dupa ID.
- **delete_movie_from_collection** – Sterge un film dintr-o colectie.

---

   **Comenzi disponibile:**
   - `login_admin`
   - `add_user`
   - `get_users`
   - `login`
   - `logout_admin`
   - `logout`
   - `delete_user`
   - `get_access`
   - `add_movie`
   - `get_movies`
   - `get_movie`
   - `delete_movie`
   - `update_movie`
   - `add_collection`
   - `get_collections`
   - `get_collection`
   - `add_movie_to_collection`
   - `delete_collection`
   - `delete_movie_from_collection`
   - `exit` – Inchide aplicatia.

---

## Detalii tehnice si observatii

- **Comunicare:**  
  Clientul foloseste socket-uri TCP pentru a comunica cu serverul, trimitand requesturi HTTP manual construite si primind raspunsuri pe care le parseaza.
- **Date:**  
  Toate datele trimise si primite sunt in format JSON.
- **Autentificare:**  
  Se folosesc cookie-uri pentru sesiune si token JWT pentru acces la operatii privilegiate.
- **Erori:**  
  Orice raspuns de eroare de la server este afisat explicit in consola.
- **Extensibilitate:**  
  Codul este modularizat cu functii separate pentru fiecare operatie si macrouri pentru a evita repetitia.

---

## Dependente

- [nlohmann/json](https://github.com/nlohmann/json) – pentru manipularea JSON in C++ (inclus ca header).
- Un server compatibil cu API-ul specificat (rulat separat).
---
*Tema pentru Protocoale de Comunicare, 2025 – Universitatea Politehnica Bucuresti*
