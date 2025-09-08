Name: Visanescu Bogdan-Emilian  
Group: 323CA

# HTTP Client

This project represents a C++ client for interacting with a REST server, allowing management of users, movies, and movie collections. The application communicates with the server using HTTP protocols and JSON data.

> **Note:** The socket/HTTP communication API implemented in `helper.cpp` is partially adapted from PCOM lab materials.

---

## Project Structure

- **client.cpp**  
  The main application file. Contains the `main()` function and all functions for interacting with the server: authentication, managing users, movies, and collections.  
- **client.hpp**  
  Contains macros for URLs, HTTP headers, and utilities to simplify the code in `client.cpp`.
- **helper.hpp / helper.cpp**  
  Helper functions for building and parsing HTTP requests and responses, as well as for socket communication with the server.
- **nlohmann/json.hpp**  
  External library for handling JSON objects in C++. Used to construct and interpret data sent to/from the server.

---

## Detailed Features

The application allows the following operations, each accessible via a command entered from the keyboard:

### User Administration (admin)
- **login_admin** – Authenticates an administrator.
- **add_user** – Adds a new user (admin only).
- **get_users** – Lists all existing users.
- **delete_user** – Deletes a user by username.
- **logout_admin** – Logs out the current administrator.

### Regular User
- **login** – Authenticates a user.
- **logout** – Logs out the current user.
- **get_access** – Obtains an access token for the movie library.

### Movies
- **add_movie** – Adds a new movie.
- **get_movies** – Lists all available movies.
- **get_movie** – Displays details of a movie by ID.
- **delete_movie** – Deletes a movie by ID.
- **update_movie** – Updates the details of a movie.

### Movie Collections
- **add_collection** – Creates a new movie collection.
- **get_collections** – Lists all available collections.
- **get_collection** – Displays details of a collection by ID.
- **add_movie_to_collection** – Adds a movie to a collection.
- **delete_collection** – Deletes a collection by ID.
- **delete_movie_from_collection** – Deletes a movie from a collection.

---

**Available Commands:**
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
- `exit` – Closes the application.

---

## Technical Details and Notes

- **Communication:**  
  The client uses TCP sockets to communicate with the server, sending manually constructed HTTP requests and parsing the responses.
- **Data:**  
  All sent and received data is in JSON format.
- **Authentication:**  
  Cookies are used for session management and JWT tokens for privileged operations.
- **Errors:**  
  Any server error response is explicitly displayed in the console.
- **Extensibility:**  
  The code is modularized with separate functions for each operation and macros to avoid repetition.

---

## Dependencies

- [nlohmann/json](https://github.com/nlohmann/json) – for JSON handling in C++ (included as a header).
- A server compatible with the specified API (run separately).

