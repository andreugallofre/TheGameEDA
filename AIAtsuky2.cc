#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Atsuky2 


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
    typedef vector<VC> VVC;

    typedef vector<structure> VS;

    struct point {
        int x, y, xa, ya;
    };

    point create_point(int x, int y, int xa, int ya) {
        point p;
        p.x = x;
        p.y = y;
        p.xa = xa;
        p.ya = ya;
        return p;
    }

    // Stores the location of orks.
    VVI ork_at;
    
    map<int, pair<int,int>> position_id_orks;

    VVC matrix;
    VVC m2;

    VS cities;
    VS paths; 

    vector<point> cami;   
    vector<point> cami2; 

    stack<string> moviments;

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

    void generate_matrix(){
        for (int i = 0; i < rows(); ++i){
            for (int j = 0; j < cols(); ++j) {
                Cell c = cell(i, j);
                // Fill the matrix with every type of cell
                if (c.type == CITY) matrix[i][j] = '#';
                else if (c.type == PATH) matrix[i][j] = '*';
                else if (c.type == WATER) matrix[i][j] = 'X';
                else if (c.type == GRASS) matrix[i][j] = 'G';
                else if (c.type == FOREST) matrix[i][j] = 'F';
                else if (c.type == SAND) matrix[i][j] = 'S';
                if (c.unit_id != -1 and unit(c.unit_id).player == me()){
                    matrix[i][j] = 'M';

                    // Add the position and the id of the ork, to a map
                    pair<int,int> p = make_pair(i,j);
                    position_id_orks[c.unit_id] = p;
                } 
            }
        }
    }

    // BFS, Get the nearest path/city form the position of the ork and it's path
    void bfs_ite(VVC& G, vector<point>& v, point& b) {
        int i = 0;
        while (true) {
            cerr << "iteration " << i << endl;
            point p = v[i];
            if (G[p.x][p.y] == '#') {
                b = p;
                return;
            } else if (G[p.x][p.y] == 'G' or G[p.x][p.y] == 'F' or G[p.x][p.y] == 'S' or G[p.x][p.y] == '*') {
                if (G[p.x-1][p.y] != 'X') v.push_back(create_point(p.x-1, p.y, p.x, p.y));
                if (G[p.x+1][p.y] != 'X') v.push_back(create_point(p.x+1, p.y, p.x, p.y));
                if (G[p.x][p.y-1] != 'X') v.push_back(create_point(p.x, p.y-1, p.x, p.y));
                if (G[p.x][p.y+1] != 'X') v.push_back(create_point(p.x, p.y+1, p.x, p.y));
                G[p.x][p.y] = 'X';
            }
            ++i;
        }
    }

    /**
    * Play method, invoked once per each round.
    */
    virtual void play () {

        matrix = VVC(rows(), VC(cols()));

        if (round() == 0){
            generate_matrix();
            ork_at = VVI(rows(), VI(cols(), -1));
            
            // Test code to print the matrix xdlol
            for (int i = 0; i < rows(); ++i){
                for (int j = 0; j < cols(); ++j) cerr << matrix[i][j];
                cerr << endl;
            }

            // Test code that print all oks and it's position
            // for (auto it=position_id_orks.begin(); it != position_id_orks.end(); it++) 
            //     cerr << "Ork id = " << it->first << " Pos i = " << it->second.first << " pos j = " << it->second.second << endl;

            // Generate a path for every ork to it's near city, move it, and 
            auto it=position_id_orks.begin();

            for (int i = 0; i < 1; ++i){
                m2 = matrix;
                point inici = create_point(it->second.first,it->second.second,0,0);
                cami.push_back(inici);
                point l = inici;

                cerr << "ok" << endl;

                bfs_ite(m2, cami, inici);

                cerr << "ok" << endl;
                
                for (int i= cami.size(); i>=0; --i) {
                    cerr << cami[i].x << " " << cami[i].y << endl;
                    if (cami[i].x == l.xa and cami[i].y == l.ya) {
                    l = cami[i];
                    cami2.push_back(l);
                    }
                }

                

                for (int i = 1; i < (int)cami2.size()-1; ++i) {
                    cerr << cami2[i].x << cami[i].y << endl;
                    if (cami2[i].x == cami2[i-1].x-1) moviments.push("TOP");
                    else if (cami2[i].x == cami2[i-1].x+1) moviments.push("BOTTOM");
                    else if (cami2[i].y == cami2[i-1].y-1) moviments.push("LEFT");
                    else if (cami2[i].x == cami2[i-1].y+1) moviments.push("RIGHT");
                }    

                while(!moviments.empty()){
                    string s = moviments.top(); moviments.pop();
                    cerr << s << endl;
                }

            }
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
