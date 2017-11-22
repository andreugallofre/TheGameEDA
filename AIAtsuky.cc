#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Atsuky


 struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
   static Player* factory () {
        return new PLAYER_NAME;
    }

  /**
   * Types and attributes for your player can be defined here.
   */


    struct structure {
        int   posi; // The id of the unit on the cell if any, -1 otherwise.
        int   posj; // If type == CITY, the id of the city, -1 otherwise.
        int   id; // If type == PATH, the id of the path, -1 otherwis
    };

    typedef vector<int> VI;
    typedef vector<VI>  VVI;

    typedef vector<char> VC;
    typedef vector<VC>  VVC;

    typedef vector<structure> VS;

    // Stores the location of orks.
    VVI ork_at;
    VS cities;
    VS paths;

    VVC matrix = VVC(rows(), VC(cols(), -1));

    // Moves ork with identifier id.
    void move(int id) {
        Unit u = unit(id);
        Pos pos = u.pos;
        // Try to move to a position within the board.
        for (int d = 0; d != DIR_SIZE; ++d) {
            Dir dir = Dir(d);
            Pos npos = pos + dir;
            if (pos_ok(npos)) {
                execute(Command(id, dir));
                return;
            }
        }
    }

    // BFS, Get the nearest path/city form the position of the ork and it's path

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {



        if (round() == 0){
            ork_at = VVI(rows(), VI(cols(), -1));

            // Get all positions of structures that appears in the board
            for (int i = 0; i < rows(); ++i){
                for (int j = 0; j < cols(); ++j) {
                    Cell c = cell(i, j);
                    
                    //Get the cities
                    if (c.type == CITY){
                        structure cs;
                        cs.posi = i;
                        cs.posj = j;
                        cs.id = c.city_id;
                        cities.push_back(cs);
                    } 

                    //Same procedure to get path's
                    else if (c.type == PATH){
                        structure ps;
                        ps.posi = i;
                        ps.posj = j;
                        ps.id = c.path_id;
                        paths.push_back(ps);
                    } 
                }
            }   

            cerr << paths.size() << " " << cities.size() << endl;

            // Show on the "error channel" all the positions that belongs to cities or paths and it's id, just a test code 
            // for (int i = 0; i < (int)cities.size(); ++i) cerr << "Test cities: PosI = " << cities[i].posi << " PosJ = " << cities[i].posj << " City_id  = " << cities[i].id << endl;
            // for (int i = 0; i < (int)paths.size(); ++i) cerr << "Test paths: PosI = " << paths[i].posi << " PosJ = " << paths[i].posj << " Path_id  = " << paths[i].id << endl;

        } 

    
        /*else {
            for (int i = 0; i < rows(); ++i)
                for (int j = 0; j < cols(); ++j) {
                    Cell c = cell(i, j);
                    if (c.type == CITY and ork_at[i][j] != c.unit_id and c.unit_id != -1 and
                    unit(c.unit_id).player != me()) {
                    cerr << "Enemy ork " << c.unit_id << " moved to CITY position " << Pos(i, j) << " of city " << c.city_id << endl;
                }
        }*/

        for (int i = 0; i < rows(); ++i)
            for (int j = 0; j < cols(); ++j)
                ork_at[i][j] = cell(i, j).unit_id; // Update for the next round.


        // Get my orks
        VI my_orks = orks(me());

        // for (int k = 0; k < int(my_orks.size()); ++k) 
        //   move(my_orks[k]); 

  }

};


/**
 * Do not modify the following line.
 */
 RegisterPlayer(PLAYER_NAME);
