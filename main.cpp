
//#include "STcpClient.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>

using namespace std ;

int cnt = 0;

struct path{
	pair<int,int> origin ;
	pair<int,int> dist ;
	vector<pair<int,int> > travel ;
	int eated ;
	int score ;
	vector<vector<int> > board ;
};


bool compare_fanout(const path &lhs , const path &rhs){
    return lhs.score > rhs.score;
}
/*
pair<int,int> operator=(pair<int,int> in){
	pair<int,int> tmp;
	tmp.first = in.first ;
	tmp.second = in.second ;
	return tmp ;
}

vector<vector<int> > operator=(vector<vector<int> > in){
	vector<vector<int> > tmp;
	tmp.resize(8);
	for ( int i = 0 ; i < 8 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ )
			tmp[i].push_back(in[i][j]);
	return tmp ; 
}
*/


vector<pair<int,int> > board_check( vector<vector<int> >& board , int color ){
	vector<pair<int,int> > tmpvec ;
	for ( int i = 0 ; i < 8 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ )
			if ( board[i][j] == color ){
				pair<int,int> tmp ;
				tmp.first = i ;
				tmp.second = j ;
				tmpvec.push_back(tmp);
			}
	return tmpvec ;
}

bool test_arrival( int color , pair<int,int> in ){
	if ( color == 1 ){
		if ( in.second == 6 || in.second == 7 ){
			return true ;
		}
		else 
			return false ;
	}
	else if ( color == 2 ){
		if ( in.second == 0 || in.second == 1 ){
			return true ;
		}
		else 
			return false ;
	}
	return false;
}

bool test_square( int color , vector<vector<int> > board , pair<int,int> in ){
	if ( board[in.first][in.second] == color && 
		 board[in.first+1][in.second] == color &&
		 board[in.first][in.second+1] == color &&
		 board[in.first+1][in.second+1] == color ){
		return true ;
	}else return false ;
}

int score( vector<vector<int>> board , int color){
	int sc = 0 ;
	int op_color = (color%2)+1 ;
	int my_chess_num = 0 , op_chess_num = 0 ;
	vector<pair<int,int> > my_chess_cood ;
// count chess amount
	for ( int i = 0 ; i < 8 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ ){
			if ( board[i][j] == color || board[i][j] == 3 || board[i][j] == 4 || board[i][j] == 5 ){ 
				//board[i][j] = color ;
				my_chess_num++ ; 
				board[i][j] = color ;
				pair<int,int> tmp ;
				tmp.first = i ;
				tmp.second = j ; 
				my_chess_cood.push_back(tmp);
			}
			if ( board[i][j] == op_color ) op_chess_num++ ;
		}
	sc += ( my_chess_num - op_chess_num ) * 2000 ; // here ------------------
	//cout << "count chess amount" << endl ;
// surrounding * 1 * 2 * 3 
// arrival test 

	for ( int i = 0 ; i < my_chess_cood.size() ; i++ ){
		int flag = 0 ;
		for ( int j = my_chess_cood[i].first-1 ; j <= my_chess_cood[i].first+1 ; j++ ){
			if ( j > 7 || j < 0 ) continue ;
			for ( int k = my_chess_cood[i].second-1 ; k <= my_chess_cood[i].second+1 ; k++ ){
				if ( k > 7 || k < 0 ) continue ;
				if ( board[j][k] == color ) flag++;
			}
		}
		if ( flag == 0 ) sc -= 100 ; // here ------------------
		for ( int j = my_chess_cood[i].first-2 ; j <= my_chess_cood[i].first+2 ; j++ ){
			if ( j > 7 || j < 0 ) continue ;
			for ( int k = my_chess_cood[i].second-2 ; k <= my_chess_cood[i].second+2 ; k++ ){
				if ( k > 7 || k < 0 ) continue ;
				if ( board[j][k] == color ) flag++;
			}
		}
		if ( flag == 0 ) sc -= 400 ; // here ------------------
		for ( int j = my_chess_cood[i].first-3 ; j <= my_chess_cood[i].first+3 ; j++ ){
			if ( j > 7 || j < 0 ) continue ;
			for ( int k = my_chess_cood[i].second-3 ; k <= my_chess_cood[i].second+3 ; k++ ){
				if ( k > 7 || k < 0 ) continue ;
				if ( board[j][k] == color ) flag++;
			}
		}
		if ( flag == 0 ) sc -= 900 ; // here ------------------

		if ( test_arrival(color,my_chess_cood[i]) == true ) sc += 1500 ;  // here ------------------
	}
	//cout << "surrounding" << endl ;
// square test 
	int num_square = 0 ;
	for ( int i = 0 ; i < 7 ; i++ ){
		for ( int j = 0 ; j < 7 ; j++ ){
			pair<int,int> tmp ;
			tmp.first = i ;
			tmp.second = j ;
			if ( test_square(color,board,tmp) == true) num_square++ ;  // here ------------------
		}
	}
	if ( num_square > 2 ) num_square = 2 ;
	sc += num_square*1800 ;
	//cout << "square" << endl ;
// far from arravial 
	for ( int i = 0 ; i < my_chess_cood.size() ; i++ ){
		if ( color == 1 ){
			sc += ( my_chess_cood[i].second * 10 );
		}
		if ( color == 2 ){
			sc += ( (7-my_chess_cood[i].second) * 10 );
		}
	}
	//cout << "arravial" << endl ;
// reward of arrvial
	/*
	for ( int i = 0 ; i < my_chess_cood.size() ; i++ ){
		if ( color == 1 ){
			if ( my_chess_cood[i].second >= 6 )
				sc += 50 ;
		}
		if ( color == 2 ){
			if ( my_chess_cood[i].second <= 1 )
				sc += 50 ;
		}
	}*/
	return sc ;
}

void move(vector<vector<int> > & board, pair<int,int> origin, pair<int,int> dist , int color ){
	board[origin.first][origin.second] = 0 ;
	board[dist.first][dist.second] = color ;
}
// double_dir 3 hori 4 vertical 5 

void copy_path( path& tmp2 , path tmp ){
	//cout << "in" << endl ;
	for ( int i = 0 ; i < 8 ; i++ ){
		vector<int> tmpvec ;
		for ( int j = 0 ; j < 8 ; j++ ){
			tmpvec.push_back( tmp.board[i][j]);
		}
		tmp2.board.push_back(tmpvec);
	}	
	for ( int i = 0 ; i < tmp.travel.size() ; i++ )
		tmp2.travel.push_back(tmp.travel[i]);
	tmp2.origin.first = tmp.origin.first ; 
	tmp2.origin.second = tmp.origin.second ;
	tmp2.dist.first = tmp.dist.first ;
	tmp2.dist.second = tmp.dist.second ;
	tmp2.eated = tmp.eated ;
	//cout << "fin " << endl ;
}

void jump_path(vector<path> &tmpvec , path tmp , int color ){
	// ------------------------------------
	//cout << tmp.dist.first << " " << tmp.dist.second << endl ;
	int op_color = (color%2)+1 ;
	if ( tmp.travel[tmp.travel.size()-1].first <= 5 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first+1][tmp.travel[tmp.travel.size()-1].second] == color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first+2][tmp.travel[tmp.travel.size()-1].second] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first+2 ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first+1][tmp2.travel[tmp2.travel.size()-1].second] = 5 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].first >= 2 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first-1][tmp.travel[tmp.travel.size()-1].second] == color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first-2][tmp.travel[tmp.travel.size()-1].second] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first-2 ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first-1][tmp2.travel[tmp2.travel.size()-1].second] = 5 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].second <= 5 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second+1] == color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second+2] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second+2 ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first][tmp2.travel[tmp2.travel.size()-1].second+1] = 4 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].second >= 2 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second-1] == color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second-2] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second-2 ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first][tmp2.travel[tmp2.travel.size()-1].second-1] = 4 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	//cout << "first part" << endl ;
	// ------------------------------------
	if ( tmp.travel[tmp.travel.size()-1].first <= 5 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first+1][tmp.travel[tmp.travel.size()-1].second] == 4 &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first+2][tmp.travel[tmp.travel.size()-1].second] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first+2 ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first+1][tmp2.travel[tmp2.travel.size()-1].second] = 3 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].first >= 2 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first-1][tmp.travel[tmp.travel.size()-1].second] == 4 &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first-2][tmp.travel[tmp.travel.size()-1].second] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first-2 ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first-1][tmp2.travel[tmp2.travel.size()-1].second] = 3 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].second <= 5 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second+1] == 5 &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second+2] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second+2 ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first][tmp2.travel[tmp2.travel.size()-1].second+1] = 3 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].second >= 2 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second-1] == 5 &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second-2] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second-2 ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first][tmp2.travel[tmp2.travel.size()-1].second-1] = 3 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	//cout << "second part" << endl ;
	// ------------------------------------
	if ( tmp.travel[tmp.travel.size()-1].first <= 5 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first+1][tmp.travel[tmp.travel.size()-1].second] == op_color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first+2][tmp.travel[tmp.travel.size()-1].second] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			//cout << "1" ;
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first+2 ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second ;
			//cout << "move" << endl ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			//cout << "after move" << endl ;
			tmp2.eated++;
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first+1][tmp2.travel[tmp2.travel.size()-1].second] = 0 ;
			//cout << "push_back" << endl ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].first >= 2 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first-1][tmp.travel[tmp.travel.size()-1].second] == op_color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first-2][tmp.travel[tmp.travel.size()-1].second] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			//cout << "2" ;
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first-2 ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.eated++;
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first-1][tmp2.travel[tmp2.travel.size()-1].second] = 0 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].second <= 5 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second+1] == op_color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second+2] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			//cout << "3" ;
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second+2 ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.eated++;
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first][tmp2.travel[tmp2.travel.size()-1].second+1] = 0 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	if ( tmp.travel[tmp.travel.size()-1].second >= 2 )
		if ( tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second-1] == op_color &&
			 tmp.board[tmp.travel[tmp.travel.size()-1].first][tmp.travel[tmp.travel.size()-1].second-2] == 0 ){
			path tmp2 ;
			copy_path(tmp2,tmp);
			//cout << "4" ;
			tmp2.dist.first = tmp.travel[tmp.travel.size()-1].first ;
			tmp2.dist.second = tmp.travel[tmp.travel.size()-1].second-2 ;
			move(tmp2.board,tmp2.travel[tmp2.travel.size()-1],tmp2.dist,color);
			tmp2.eated++;
			tmp2.board[tmp2.travel[tmp2.travel.size()-1].first][tmp2.travel[tmp2.travel.size()-1].second-1] = 0 ;
			tmp2.travel.push_back(tmp2.dist);
			tmpvec.push_back(tmp2);
			jump_path(tmpvec,tmp2,color);
		}
	//cout << "third part" << endl ;
}

vector<path> find_path( vector<vector<int> > board , pair<int,int> origin , int color ){
	//cout << origin.first << " " << origin.second << endl ;
	vector<path> tmpvec ;
	//pair<int,int> tmp ;
	if (  origin.first+1 < 8 )
		if ( board[origin.first+1][origin.second] == 0 ){
			path tmp ;
			tmp.origin = origin ;
			tmp.dist.first = origin.first+1 ;
			tmp.dist.second = origin.second ;
			tmp.travel.push_back(origin);
			tmp.travel.push_back(tmp.dist);
			tmp.eated = 0 ;
			tmp.board = board ;
			move(tmp.board,origin,tmp.dist,color);
			tmpvec.push_back(tmp);
		}
	if ( origin.first-1 >= 0 )
		if ( board[origin.first-1][origin.second] == 0 ){
			path tmp ;
			tmp.origin = origin ;
			tmp.dist.first = origin.first-1 ;
			tmp.dist.second = origin.second ;
			tmp.travel.push_back(origin);
			tmp.travel.push_back(tmp.dist);
			tmp.eated = 0 ;
			tmp.board = board ;
			move(tmp.board,origin,tmp.dist,color);
			tmpvec.push_back(tmp);
		}
	if ( origin.second+1 < 8 ) 
		if ( board[origin.first][origin.second+1] == 0 ){
			path tmp ;
			tmp.origin = origin ;
			tmp.dist.first = origin.first ;
			tmp.dist.second = origin.second+1 ;
			tmp.travel.push_back(origin);
			tmp.travel.push_back(tmp.dist);
			tmp.eated = 0 ;
			tmp.board = board ;
			move(tmp.board,origin,tmp.dist,color);
			tmpvec.push_back(tmp);
		} 
	if ( origin.second-1 >= 0 )
		if ( board[origin.first][origin.second-1] == 0 ){
			path tmp ;
			tmp.origin = origin ;
			tmp.dist.first = origin.first ;
			tmp.dist.second = origin.second-1 ;
			tmp.travel.push_back(origin);
			tmp.travel.push_back(tmp.dist);
			tmp.eated = 0 ;
			tmp.board = board ;
			move(tmp.board,origin,tmp.dist,color);
			tmpvec.push_back(tmp);
		}
	//cout << "after single path" << endl ;
	path tmp ;
	tmp.origin = origin ;
	tmp.dist = origin ;
	tmp.eated = 0 ;
	tmp.travel.push_back(origin);
	tmp.board = board ;
	jump_path(tmpvec,tmp,color);
	//cout << "jump_path" << endl ;
	//cout << "size " << tmpvec.size() << endl ;
	for ( int i = 0 ; i < tmpvec.size() ; i++ ){
		tmpvec[i].score = score( tmpvec[i].board , color );
	}
	//cout << "score" << endl ;
	sort(tmpvec.begin(),tmpvec.end(),compare_fanout);
	vector<path> final;
	for ( int i = 0 ; i < tmpvec.size() ; i++ ){
		if ( i > 4 )break ;
		final.push_back(tmpvec[i]);
	}
	//cout << "final" << endl ;
	return final ;
}

vector<path> find_top_sc_of_each_chess( vector<vector<int> > board , vector<pair<int,int> > site , int color ){
	vector<path> the_choose_path ;
	for ( int i = 0 ; i < site.size() ; i++ ){
		vector<path> tmp_path ;
		tmp_path = find_path( board , site[i] , color ) ;
		for ( int j = 0 ; j < tmp_path.size() ; j++ )
			the_choose_path.push_back(tmp_path[j]);
	}
	//cout << "after find_path" << endl ;
	sort(the_choose_path.begin(),the_choose_path.end(),compare_fanout);
	vector<path> final ;
	for ( int i = 0 ; i < the_choose_path.size() ; i++ ){
		if ( i > 5 ) break; 
		//cout << the_choose_path[i].score << " " ;
		final.push_back(the_choose_path[i]);
	}
	//cout << endl ;
	return final ;
}
/*
vector<string> board_to_string( vector<vector<int> > board , int color ){
	vector<string> tmp ;
	for ( int i = 0 ; i < 8 ; i++ ){
		string tmpstring ;
		for ( int j = 0 ; j < 8 ; j++ ){
			if ( board[i][j] == color )tmpstring.push_back("1");
		}
		tmp.push_back(tmpstring);
	}
	return tmp ;
}
*/
bool the_first_path(vector<vector<int> > board , int color){
	//vector<string> string_board ;
	//string_board = board_to_string(board,color);
	if ( color == 1 ){
		if ( board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1 &&
			 board[2][0] == 1 && board[3][1] == 1 && board[4][2] == 1 &&
			 board[4][0] == 1 && board[5][1] == 1 &&
			 board[6][0] == 1 ){
			return true ;
		}else return false ; 
	}
	if ( color == 2 ){
		if ( board[3][5] == 2 && board[2][6] == 2 && board[1][7] == 2 &&
			 board[5][5] == 2 && board[4][6] == 2 && board[3][7] == 2 &&
			 					 board[6][6] == 2 && board[5][7] == 2 &&
			 					 					 board[7][7] == 2 ){
			return true ;
		}else return false ; 
	}
	return false ;
}

bool the_second_path( vector<vector<int> > board , int color){
	if ( color == 1 ){
		if ( board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1 &&
			 board[2][0] == 1 && board[3][1] == 1 && board[4][2] == 1 &&
			 board[4][0] == 1 && board[5][1] == 1 &&
			 board[5][0] == 1 ){
			return true ;
		}else return false ; 
	}
	if ( color == 2 ){
		if ( board[3][5] == 2 && board[2][6] == 2 && board[2][7] == 2 &&
			 board[5][5] == 2 && board[4][6] == 2 && board[3][7] == 2 &&
			 					 board[6][6] == 2 && board[5][7] == 2 &&
			 					 					 board[7][7] == 2 ){
			return true ;
		}else return false ; 
	}
	return false ;
}

bool the_third_path(vector<vector<int> > board , int color){
	if ( color == 1 ){
		if ( board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1 &&
			 board[2][0] == 1 && board[3][1] == 1 && board[4][2] == 1 &&
			 board[4][0] == 1 && board[5][1] == 1 && board[5][2] == 1 ){
			return true ;
		}else return false ; 
	}
	if ( color == 2 ){
		if ( board[2][5] == 2 && board[2][6] == 2 && board[3][7] == 2 &&
			 board[3][5] == 2 && board[4][6] == 2 && board[5][7] == 2 &&
			 board[5][5] == 2 && board[6][6] == 2 && board[7][7] == 2 ){
			return true ;
		}else return false ; 
	}
	return false ;
}

bool the_forth_path(vector<vector<int> > board , int color){
	if ( color == 1 ){
		if ( board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1 &&
			 board[2][0] == 1 && board[3][1] == 1 && board[4][2] == 1 &&
			 				     board[4][1] == 1 && board[5][2] == 1 &&
			 				     board[5][1] == 1){
			return true ;
		}else return false ; 
	}
	if ( color == 2 ){
		if ( board[2][5] == 2 && board[2][6] == 2 &&
			 board[3][5] == 2 && board[3][6] == 2 && board[5][7] == 2 &&
			 board[5][5] == 2 && board[4][6] == 2 && board[7][7] == 2 &&
			 				     board[6][6] == 2){
			return true ;
		}else return false ; 
	}
	return false ;
}

bool the_fifth_path(vector<vector<int> > board , int color){
	if ( color == 1 ){
		if ( board[1][0] == 1 && board[1][1] == 1 && board[2][2] == 1 &&
			 board[2][0] == 1 && board[3][1] == 1 && board[4][2] == 1 &&
			 				     board[4][1] == 1 && board[5][2] == 1 &&
			 				     board[5][1] == 1){
			return true ;
		}else return false ; 
	}
	if ( color == 2 ){
		if ( board[2][5] == 2 && board[2][6] == 2 &&
			 board[3][5] == 2 && board[3][6] == 2 && board[5][7] == 2 &&
			 board[5][5] == 2 && board[4][6] == 2 && board[6][7] == 2 &&
			 				     board[6][6] == 2){
			return true ;
		}else return false ; 
	}
	return false ;
}

int best_score( vector<vector<int> > board , int color , int deep ){
	//cout << "cnt====" << cnt << endl ;
	cnt++ ;
	vector<pair<int,int> > site ;
	if ( color == 1 ){
		site = board_check(board,color);
	}
	if ( color == 2 ){
		site = board_check(board,color);
	}
	vector<path> pool = find_top_sc_of_each_chess( board , site , color );
	if (pool.size() == 0) return -10000;
	if ( deep == 4 ){
		return pool[0].score ;
	}
	//cout << endl << "deep " << deep << endl ;
	int max = best_score( pool[0].board , (color%2)+1 , deep+1 ) ;
	for ( int i = 1 ; i < pool.size() ; i++ ){
		int tmp = best_score( pool[i].board , (color%2)+1 , deep+1 ) ;
		if ( tmp > max ) max = tmp;
	}
	return max ;
}

vector<vector<int> > travel_to_step(path in){
	vector<vector<int> > ans ;
	for ( int i = 0 ; i < in.travel.size() ; i++ ){
		vector<int> tmp ;
		tmp.push_back(in.travel[i].first);
		tmp.push_back(in.travel[i].second);
		ans.push_back(tmp);
	}
	return ans ;
}

pair<int,int> step_to_pair( vector<int> step ){
	pair<int,int> tmp ;
	tmp.first = step[0] ;
	tmp.second = step[1] ;
	return tmp ;
}

void turn_board_to_normal(vector<vector<int>>& board , int color ){
	for( int i = 0 ; i < 8 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ )
			if ( board[i][j] == 3 || board[i][j] == 4 || board[i][j] == 5 ) board[i][j] = color ;
}

void print_board( vector<vector<int> >  board ){
	cout << "------------------------" << endl ;
	for ( int i = 0 ; i < 8 ; i++ ){
		cout << "|" ;
		for ( int j = 0 ; j < 8 ; j++ ){
			cout << setw(2) << board[i][j] << "|" ;
		}
		cout << endl << "-------------------------" << endl ; 
	}
}

vector<vector<int> > doit( vector<vector<int> > & board , int color){
	//cout << "doit" << endl ;
	if ( the_first_path(board,color) == true ){
		//cout << "the_first_path" << endl ;
		vector<vector<int> > final;
		if ( color == 1 ){
			vector<int> tmp ;
			tmp.push_back(6);
			tmp.push_back(0);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(5);
			tmp.push_back(0);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
			return final ;
		}
		if ( color == 2 ){
			vector<int> tmp ;
			tmp.push_back(1);
			tmp.push_back(7);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(2);
			tmp.push_back(7);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
			return final ;
		}
	}else if ( the_second_path(board,color) == true ){
		//cour << the_second_path(board,color) << endl ;
		//cout << "the_second_path" << endl ;
		vector<vector<int> > final;
		if ( color == 1 ){
			vector<int> tmp ;
			tmp.push_back(5);
			tmp.push_back(0);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(5);
			tmp.push_back(2);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
			return final ;
		}
		if ( color == 2 ){
			vector<int> tmp ;
			tmp.push_back(2);
			tmp.push_back(7);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(2);
			tmp.push_back(5);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
			return final ;
		}
	}else if ( the_third_path(board,color) == true ){
		vector<vector<int> > final;
		if ( color == 1 ){
			vector<int> tmp ;
			tmp.push_back(4);
			tmp.push_back(0);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(4);
			tmp.push_back(1);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
		}
		if ( color == 2 ){
			vector<int> tmp ;
			tmp.push_back(3);
			tmp.push_back(7);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(3);
			tmp.push_back(6);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
		}
		return final ;
	}else if ( the_forth_path(board,color) == true ){
		vector<vector<int> > final;
		if ( color == 1 ){
			vector<int> tmp ;
			tmp.push_back(0);
			tmp.push_back(0);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(1);
			tmp.push_back(0);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
		}
		if ( color == 2 ){
			vector<int> tmp ;
			tmp.push_back(7);
			tmp.push_back(7);
			final.push_back(tmp);
			tmp.clear();
			tmp.push_back(6);
			tmp.push_back(7);
			final.push_back(tmp);
			move(board, step_to_pair(final[0]), step_to_pair(final[1]), color);
		}
		return final ;
	}else{
		//cout << "doit in else" << endl ;
		vector<pair<int,int> > site ;
		if ( color == 1 ){
			site = board_check(board,color);
		}
		if ( color == 2 ){
			site = board_check(board,color);
		}
		//cout << "after board_check " << endl ;
		vector<path> pool = find_top_sc_of_each_chess( board , site , color );
		cout << "pool: " << pool.size() << endl ;
		//cout << "find_top_sc_of_each_chess" << endl ;
		int num = 0 ;
		int max = best_score( pool[0].board , (color%2)+1 , 1 ) ;
		cout << "max: " << max << "  board: " << score(pool[0].board,color) <<  endl ;
		for ( int i = 1 ; i < pool.size() ; i++ ){
			int tmp = best_score( pool[i].board , (color%2)+1 , 1 ) ;
			cout << "max: " << max << "  board: " << score(pool[i].board,color) <<  endl ;
			//cout << "boradlook " << endl ;
			//print_board(pool[i].board);
			if ( tmp > max ) max = tmp , num = i ;
		}
		//cout << "after best_score " << endl ;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if ( pool[num].board[i][j] == 0 ) board[i][j] = pool[num].board[i][j];
				else if ( pool[num].board[i][j] == 1 ) board[i][j] = pool[num].board[i][j];
				else if ( pool[num].board[i][j] == 2 ) board[i][j] = pool[num].board[i][j];
				else board[i][j] = color;
			}
		}
		cout << "max: " << max << "  board: " << score(board,color) <<  endl ;
		cnt = 0 ;
		return travel_to_step( pool[num] );
	}
}



std::vector<std::vector<int> > GetStep(std::vector<std::vector<int> >& board, bool is_black) {
	//cout << "GetStep" << endl ;
	int color , op_color ;
	if ( is_black == true ) color = 1 , op_color = 2 ;
	else color = 2 , op_color = 1 ;
	std::vector<std::vector<int> > step;
	step = doit( board , color );
	return step;
}



bool test_final( vector<vector<int> > board ){
	int flag = 0 ;
	for ( int i = 0 ; i < 8 ; i++ ){
		for ( int j = 0 ; j < 6 ; j++ ){
			if ( board[i][j] == 1 ) flag = 1 ; 
		}
	}
	if ( flag == 0 ) return true ;
	for ( int i = 0 ; i < 8 ; i++ ){
		for ( int j = 2 ; j < 8 ; j++ ){
			if ( board[i][j] == 2 ) flag = 2 ; 
		}
	}
	if ( flag == 1 ) return true ;
	return false ;
}

void init_board(vector<vector<int> > & board ){
	for ( int i = 0 ; i < 8 ; i++ ){
		vector<int> tmp ;
		for ( int j = 0 ; j < 8 ; j++ ){
			tmp.push_back(0);
		}
		board.push_back(tmp);
	}
	// black
	board[0][0] = 1 ;
	board[2][0] = 1 ;
	board[4][0] = 1 ;
	board[6][0] = 1 ;
	board[1][1] = 1 ;
	board[3][1] = 1 ;
	board[5][1] = 1 ;
	board[2][2] = 1 ;
	board[4][2] = 1 ;
	// white
	board[3][5] = 2 ;
	board[5][5] = 2 ;
	board[2][6] = 2 ;
	board[4][6] = 2 ;
	board[6][6] = 2 ;
	board[1][7] = 2 ;
	board[3][7] = 2 ;
	board[5][7] = 2 ;
	board[7][7] = 2 ;
}

void show_path( vector<vector<int>> in ){
	for ( int i = 0 ; i < in.size() ; i++ ){
		cout << "<" << in[i][0] << "," << in[i][1] << "> "  ;
	}
	cout << endl ;
}

int main() {
	std::vector<std::vector<int> > board, step;
	int step_cnt = 0 ; 
	cout << "init_board" << endl ;
	init_board(board);
	cout << "start" << endl ;
	while ( step_cnt < 200 && test_final(board) == false ){
		cout << step_cnt << " " << (step_cnt%2)+1 << endl ;
		string tmpstring ;
		step = GetStep(board, (step_cnt+1)%2 );
		if ( ((step_cnt%2)+1) == 1 )cout << endl << "black" << endl ;
		else cout << endl << "white" << endl ;
		//print_board(board);
		//move(board,step_to_pair(step[0]),step_to_pair(step[step.size()-1]),(step_cnt%2)+1);
		show_path(step);
		print_board(board);
		cout << "score: " << score(board,((step_cnt%2)+1)) << endl ;
		step_cnt++;
		cin >> tmpstring ;
	}
}
