//#include <pqxx/pqxx>
#include "../repository.cpp"
#include <pqxx/pqxx>

class PostgresRepository : Repository {
    void delete_message() override;
};

void PostgresRepository::delete_message() {
    }

