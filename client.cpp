#include "client.hpp"
#include "helper.hpp"

std::string admin_cookie;
std::string user_cookie;
std::string token;
using namespace std;

HttpResponse send_json_request(int sockfd, const std::string& method, const std::string& url,
                              const std::map<std::string, std::string>& headers,
                              const std::string& body = "") {
    std::string request = build_http_request(method, url, SERVER_IP, headers, body);
    send_to_server(sockfd, (char *)request.c_str());
    std::string response(receive_from_server(sockfd));
    json headers_json, body_json;
    parse_http_response(response, headers_json, body_json);
    return {headers_json, body_json};
}

void login_admin(int sockfd){
    string username, password;
    cout << "username=";
    cin >> username;
    cout << "password=";
    cin >> password;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", admin_cookie}
    };

    std::ostringstream body_stream;
    body_stream << "{\n"
                << "  \"username\": \"" << username << "\",\n"
                << "  \"password\": \"" << password << "\"\n"
                << "}";
    std::string body = body_stream.str();

    auto resp = send_json_request(sockfd, "POST", "/api/v1/tema/admin/login", headers, body);

    if (check_http_error(resp.headers)) return;

    if(!resp.body.contains("error")){
        std::string cookie_full = resp.headers["Set-Cookie"];
        size_t semicolon_pos = cookie_full.find(';');
        admin_cookie = cookie_full.substr(0, semicolon_pos);
        printf("SUCCESS: Admin logat cu succes!\n");
        return;
    }
    cout << "ERROR: " << resp.body["error"] << "\n";
}

void add_user(int sockfd){
    string username, password;
    cout << "username=";
    cin >> username;
    cout << "password=";
    cin >> password;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", admin_cookie}
    };

    std::ostringstream body_stream;
    body_stream << "{\n"
                << "  \"username\": \"" << username << "\",\n"
                << "  \"password\": \"" << password << "\"\n"
                << "}";
    std::string body = body_stream.str();

    auto resp = send_json_request(sockfd, "POST", "/api/v1/tema/admin/users", headers, body);

    if (check_http_error(resp.headers)) return;

    if(!resp.body.contains("error")){
        printf("SUCCESS: 200 - OK - Utilizator adăugat cu succes!\n");
        return;
    }
    cout << "ERROR: " << resp.body["error"] << "\n";
}

void get_users(int sockfd) {
    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", admin_cookie}
    };

    auto resp = send_json_request(sockfd, "GET", "/api/v1/tema/admin/users", headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Lista utilizatorilor\n");
        int idx = 1;
        for (const auto& user : resp.body["users"]) {
            std::cout << "#" << idx << " "
                      << user["username"].get<std::string>() << ":"
                      << user["password"].get<std::string>() << std::endl;
            idx++;
        }
        return;
    }
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void login(int sockfd) {
    string admin_username, username, password;
    cout << "admin_username=";
    cin >> admin_username;
    cout << "username=";
    cin >> username;
    cout << "password=";
    cin >> password;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", user_cookie}
    };

    std::ostringstream body_stream;
    body_stream << "{\n"
                << "  \"admin_username\": \"" << admin_username << "\",\n"
                << "  \"username\": \"" << username << "\",\n"
                << "  \"password\": \"" << password << "\"\n"
                << "}";
    std::string body = body_stream.str();

    auto resp = send_json_request(sockfd, "POST", "/api/v1/tema/user/login", headers, body);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Utilizator autentificat cu succes!\n");
        std::string cookie_full = resp.headers["Set-Cookie"];
        size_t semicolon_pos = cookie_full.find(';');
        user_cookie = cookie_full.substr(0, semicolon_pos);
        return;
    }
    
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void logout_admin(int sockfd) {
    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", admin_cookie}
    };

    auto resp = send_json_request(sockfd, "GET", "/api/v1/tema/admin/logout", headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Admin delogat cu succes!\n");
        admin_cookie.clear();
        return;
    }
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void logout(int sockfd) {
    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", user_cookie}
    };

    auto resp = send_json_request(sockfd, "GET", "/api/v1/tema/user/logout", headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Utilizator delogat cu succes!\n");
        user_cookie.clear();
        return;
    }
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void delete_user(int sockfd) {
    string username;
    cout << "username=";
    cin >> username;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Cookie", admin_cookie}
    };

    std::string url = "/api/v1/tema/admin/users/" + username;
    auto resp = send_json_request(sockfd, "DELETE", url, headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Utilizator sters cu succes!\n");
        return;
    }
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void get_access(int sockfd) {
    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    auto resp = send_json_request(sockfd, "GET", "/api/v1/tema/library/access", headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Acces la library obținut!\n");
        token = resp.body["token"];
        return;
    }
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void add_movie(int sockfd) {
    string title, description;
    int  year;
    double rating;
    cout << "title=";
    cin.ignore();
    getline(cin, title);

    cout << "year=";
    cin >> year;
    cin.ignore();

    cout << "description=";
    getline(cin, description);
    cout << "rating=";
    cin >> rating;
    cin.ignore();

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::ostringstream body_stream;
    body_stream << "{\n"
                << "  \"title\": \"" << title << "\",\n"
                << "  \"year\": " << year << ",\n"
                << "  \"description\": \"" << description << "\",\n"
                << "  \"rating\": " << rating << "\n"
                << "}";
    std::string body = body_stream.str();

    auto resp = send_json_request(sockfd, "POST", "/api/v1/tema/library/movies", headers, body);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Film adăugat\n");
        return;
    }

    std::cout << "ERROR: " << resp.body["error"] << std::endl;
}

void get_movies(int sockfd) {
    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    auto resp = send_json_request(sockfd, "GET", "/api/v1/tema/library/movies", headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Lista filmelor\n");
        for (const auto& movie : resp.body["movies"]) {
            std::cout << "#" <<  movie["id"] << " "
                      << movie["title"].get<std::string>() << std::endl;
        }
        return;
    }
    std::cout << "ERROR: " << resp.body["error"] << std::endl; 
}

void get_movie(int sockfd) {
    std::string id;
    cout << "id=";
    cin >> id;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::string url = "/api/v1/tema/library/movies/" + id;
    auto resp = send_json_request(sockfd, "GET", url, headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Detalii film\n");
        std::cout << "title: " << resp.body["title"].get<std::string>() << std::endl;
        std::cout << "year: " << resp.body["year"].get<int>() << std::endl;
        std::cout << "description: " << resp.body["description"].get<std::string>() << std::endl;
        std::cout << "rating: " << resp.body["rating"].get<std::string>() << std::endl;
        return;
    }

    std::cout << "ERROR: " <<   resp.body["error"] << std::endl;

}

void delete_movie(int sockfd) {
    std::string id;
    cout << "id=";
    cin >> id;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::string url = "/api/v1/tema/library/movies/" + id;
    auto resp = send_json_request(sockfd, "DELETE", url, headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Film șters cu succes\n");
        return;
    }

    std::cout << "ERROR: " << resp.body["error"] << std::endl;

}

void update_movie(int sockfd) {
    std::string id, title, description;
    int year;
    double rating;

    cout << "id=";
    cin >> id;
    cin.ignore();

    cout << "title=";
    getline(cin, title);

    cout << "year=";
    cin >> year;
    cin.ignore();

    cout << "description=";
    getline(cin, description);

    cout << "rating=";
    cin >> rating;
    cin.ignore();

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::ostringstream body_stream;
    body_stream << "{\n"
                << "  \"title\": \"" << title << "\",\n"
                << "  \"year\": " << year << ",\n"
                << "  \"description\": \"" << description << "\",\n"
                << "  \"rating\": " << rating << "\n"
                << "}";
    std::string body = body_stream.str();

    std::string url = "/api/v1/tema/library/movies/" + id;
    auto resp = send_json_request(sockfd, "PUT", url, headers, body);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Film actualizat\n");
        return;
    }
    
    std::cout << "ERROR: " << resp.body["error"] << std::endl;
    
}

void add_collection(int sockfd) {
    std::string title;
    int num_movies;
    std::vector<int> movie_ids;

    cout << "title=";
    cin.ignore();
    getline(cin, title);

    cout << "num_movies=";
    cin >> num_movies;

    for (int i = 0; i < num_movies; ++i) {
        int movie_id;
        cout << "movie_id[" << i << "]=";
        cin >> movie_id;
        movie_ids.push_back(movie_id);
    }

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    json body_json;
    body_json["title"] = title;
    std::string body = body_json.dump();

    auto resp = send_json_request(sockfd, "POST", "/api/v1/tema/library/collections", headers, body);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        std::string collection_id = std::to_string(resp.body["id"].get<int>());
        for (int movie_id : movie_ids) {
            std::map<std::string, std::string> add_headers = {
                {"Content-Type", "application/json"},
                {"Authorization", "Bearer " + token},
                {"Cookie", user_cookie}
            };
            json add_body_json;
            add_body_json["id"] = movie_id;
            std::string add_body = add_body_json.dump();
            std::string url = "/api/v1/tema/library/collections/" + collection_id + "/movies";
            auto add_resp = send_json_request(sockfd, "POST", url, add_headers, add_body);
            if (check_http_error(add_resp.headers)) return;
        }
        printf("SUCCESS: Colecție adăugată\n");
        return;
    }
    std::cout << "ERROR: " << resp.body["error"].get<std::string>() << std::endl;
}

void get_collections(int sockfd) {
    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    auto resp = send_json_request(sockfd, "GET", "/api/v1/tema/library/collections", headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Lista colecțiilor\n");
        for (const auto& collection : resp.body["collections"]) {
            std::cout << "#" << collection["id"] << ": " << collection["title"].get<std::string>() << std::endl;
        }
        return;
    }
    std::cout << "ERROR: " << resp.body["error"].get<std::string>() << std::endl;
}

void get_collection(int sockfd) {
    std::string id;
    cout << "id=";
    cin >> id;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::string url = "/api/v1/tema/library/collections/" + id;
    auto resp = send_json_request(sockfd, "GET", url, headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Detalii colecție\n");
        std::cout << "title: " << resp.body["title"].get<std::string>() << std::endl;
        std::cout << "owner: " << resp.body["owner"].get<std::string>() << std::endl;
        for (const auto& movie : resp.body["movies"]) {
            std::cout << "#" << movie["id"].get<int>() << ": " << movie["title"].get<std::string>() << std::endl;
        }
        return;
    }

    std::cout << "ERROR: " << resp.body["error"].get<std::string>() << std::endl;
    
}

void add_movie_to_collection(int sockfd) {
    std::string collection_id, movie_id;
    cout << "collection_id=";
    cin >> collection_id;
    cout << "movie_id=";
    cin >> movie_id;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::ostringstream body_stream;
    body_stream << "{\n"
                << "  \"id\": " << movie_id << "\n"
                << "}";
    std::string body = body_stream.str();

    std::string url = "/api/v1/tema/library/collections/" + collection_id + "/movies";
    auto resp = send_json_request(sockfd, "POST", url, headers, body);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Film adăugat în colecție\n");
        return;
    }

    std::cout << "ERROR: " << resp.body["error"].get<std::string>() << std::endl;

}

void delete_collection(int sockfd) {
    std::string collection_id;
    cout << "id=";
    cin >> collection_id;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::string url = "/api/v1/tema/library/collections/" + collection_id;
    auto resp = send_json_request(sockfd, "DELETE", url, headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Colecție ștearsă cu succes\n");
        return;
    }
    std::cout << "ERROR: " << resp.body["error"].get<std::string>() << std::endl;
}

void delete_movie_from_collection(int sockfd) {
    std::string collection_id, movie_id;
    cout << "collection_id=";
    cin >> collection_id;
    cout << "movie_id=";
    cin >> movie_id;

    std::map<std::string, std::string> headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer " + token},
        {"Cookie", user_cookie}
    };

    std::string url = "/api/v1/tema/library/collections/" + collection_id + "/movies/" + movie_id;
    auto resp = send_json_request(sockfd, "DELETE", url, headers);

    if (check_http_error(resp.headers)) return;

    if (!resp.body.contains("error")) {
        printf("SUCCESS: Film șters din colecție\n");
        return;
    }
    std::cout << "ERROR: " << resp.body["error"].get<std::string>() << std::endl;
}
int main() {
    
    int sockfd = open_connection((char *)SERVER_IP, SERVER_PORT, AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        cerr << "Error openning the connection." << endl;
        return 1;
    }

    while(true){
        string command;
        cin >> command;
        if(command == "login_admin"){
            login_admin(sockfd);
        } else if(command == "add_user") {
            add_user(sockfd);
        } else if(command == "get_users") {
            get_users(sockfd);
        } else if(command == "login") {
            login(sockfd);
        } else if(command == "logout_admin") {
            logout_admin(sockfd);
        } else if(command == "logout") {
            logout(sockfd);
        } else if(command == "delete_user") {
            delete_user(sockfd);
        } else if(command == "get_access") {
            get_access(sockfd);
        }else if(command == "add_movie") {
            add_movie(sockfd);
        }else if(command == "get_movies"){
            get_movies(sockfd);
        }else if(command == "get_movie"){
            get_movie(sockfd);
        } else if(command == "delete_movie"){
            delete_movie(sockfd);
        } else if(command == "update_movie"){
            update_movie(sockfd);
        } else if(command == "add_collection"){
            add_collection(sockfd);
        } else if(command == "get_collections"){
            get_collections(sockfd);
        } else if(command == "get_collection"){
            get_collection(sockfd);
        } else if(command == "add_movie_to_collection"){
            add_movie_to_collection(sockfd);
        } else if(command == "delete_collection"){
            delete_collection(sockfd);
        } else if(command == "delete_movie_from_collection"){
            delete_movie_from_collection(sockfd);
        } else if(command == "exit"){
            break;
        }
    }

    close(sockfd);
    return 0;
}