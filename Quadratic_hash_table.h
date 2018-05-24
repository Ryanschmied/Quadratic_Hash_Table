/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  rmschmie@uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    - sarlic@uwaterloo.ca
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    - enruizno@edu.uwaterloo.ca (used test file)
 *    - y327lin@edu.uwaterloo.ca (used test file)
 *    - zsnafzig@edu.uwaterloo.ca (used test file)
 *****************************************/

#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Exception.h"
#include "ece250.h"

enum bin_state_t { UNOCCUPIED, OCCUPIED, ERASED };

template <typename Type>
class Quadratic_hash_table {
private:
    int numLoad;
    int count;
    int power;
    int array_size;
    int mask;
    Type *array;
    bin_state_t *occupied;
    
    int hash( Type const & ) const;
    
public:
    Quadratic_hash_table( int = 5 );
    ~Quadratic_hash_table();
    int size() const;
    int capacity() const;
    double load_factor() const;
    bool empty() const;
    bool member( Type const & ) const;
    Type bin( int ) const;
    
    void print() const;
    
    void insert( Type const & );
    bool erase( Type const & );
    void clear();
    
    // Friends
    
    template <typename T>
    friend std::ostream &operator<<( std::ostream &, Quadratic_hash_table<T> const & );
};
//Constructors

template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table( int m ):
numLoad(0),count( 0 ), power( m ),
array_size( 1 << power ),
mask( array_size - 1 ),
array( new Type[array_size] ),
occupied( new bin_state_t[array_size] ) {
    //Check if power is it too low, set to default of 5 if so
    if(m<5)
        m=5;
    for ( int i = 0; i < array_size; ++i ) {
        occupied[i] = UNOCCUPIED;
    }
}
template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table(){
    delete []array;
    delete []occupied;
}
//Accessors

//Return how many entries are in the hash table
template <typename Type>
int Quadratic_hash_table<Type>::size() const{
    return count;
}
//Return capacity of has table
template <typename Type>
int Quadratic_hash_table<Type>::capacity() const{
    return array_size;
}
//Return load factor of hash table (# entries/capacity)
template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const{
    return numLoad/(double)array_size;
    
}
//Return true if hash table is empty otherwise return false
template <typename Type>
bool Quadratic_hash_table<Type>::empty() const{
    if(count == 0)
        return true;
    return false;
}
//Return true if obj is a member of the hash table, if not return false
template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj) const{
    //Start search at hashed index of object
    int initial = hash(obj);
    for(int k = 0; k < array_size; ++k){
        //Quadratic probing to handle collision
        initial = (initial + k) % array_size;
        //Check if bin at index holds object
        if(occupied[initial] == OCCUPIED && array[initial]==obj){
            return true;
        }
    }
    return false;
}
//Return object at bin n
template <typename Type>
Type Quadratic_hash_table<Type>::bin(int n) const{
    return array[n];
}
template <typename Type>
void Quadratic_hash_table<Type>::print() const{
    
}

//Mutators

//Insert obj into hash table
template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj){
    if(count == array_size)
        throw overflow();
    else if(member(obj))
        return;
    else{
        //Start search at hashed index of object
        int initial = hash(obj);
        for(int k = 0; k < array_size; ++k){
            //Quadratic probing to handle collision
            initial = (initial + k) % array_size;
            //Check if bin at index is occupied, if not insert may take place
            if(occupied[initial] != OCCUPIED){
                if(occupied[initial] != ERASED){
                    occupied[initial] = OCCUPIED;
                    array[initial] = obj;
                    count++;
                    numLoad++;
                    break;
                }
                else{
                    occupied[initial] = OCCUPIED;
                    array[initial] = obj;
                    count++;
                    break;
                }
            }
        }
    }
}
//Erase obj from the hash table
template <typename Type>
bool Quadratic_hash_table<Type>::erase( Type const &obj){
    
        //Start search at hashed index of object
        int initial = hash(obj);
        for(int k = 0; k < array_size; ++k){
            //Quadratic probing to handle collision
            initial = (initial + k) % array_size;
            //Check if bin at index holds object to be erased
            if(occupied[initial] == OCCUPIED && array[initial] == obj){
                occupied[initial] = ERASED;
                count--;
                return true;
            }
        }
    return false;
}
//Clear array by setting all values of occupied array to UNOCUPIED and set count to 0
template <typename Type>
void Quadratic_hash_table<Type>::clear(){
    count = 0;
    numLoad = 0;
    for ( int i = 0; i < array_size; ++i ) {
        occupied[i] = UNOCCUPIED;
    }
}
//Hash object to map it to index
template <typename Type>
int Quadratic_hash_table<Type>::hash( Type const &obj)const{
    int i = static_cast<int>(obj);
    return i & mask;
}

template <typename T>
std::ostream &operator<<( std::ostream &out, Quadratic_hash_table<T> const &hash ) {
    for ( int i = 0; i < hash.capacity(); ++i ) {
        if ( hash.occupied[i] == UNOCCUPIED ) {
            out << "- ";
        } else if ( hash.occupied[i] == ERASED ) {
            out << "x ";
        } else {
            out << hash.array[i] << ' ';
        }
    }
    
    return out;
}

#endif

