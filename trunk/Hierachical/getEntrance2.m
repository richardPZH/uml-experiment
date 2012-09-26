function [ Entrance ] = getEntrance2( E1 , trGist , trlabels , bit , method )
%
% This function performs the second hashing, its purpose is to hash the hashed 
% general class into isolated classes. e.g.
%      living things -> bird cat deer dog horse frog
%      things on in the air -> bird airplane 
%
% Input:
%     E1, the first level Entrance , it is a cell( 1 , 2 ) , 
%         cell( 1 , 1 ) stores the binary code matrix
%         cell( 1 , 2 ) stores a cell contain indice of the trGist
%     trGist, the nx320 CIFAR , Gist represented images as a learning samples
% 	  trlabels, the nx1 CIFAR labels, 0~9
%     bit, how many bit is assigned to this hash code
%     method, e.g 'ITQ' 'OURSITQ' 'ITQRR'
%
%
% Output:
%     Entrance -> the second level Entrance, it is a cell( n , 3 ) , 
%         cell( x , 1 ) stores the w1 r1 cp1 of the E1{ x , : }
%         cell( x , 2 ) stores the binary code matrix b
%         cell( x , 3 ) stores a cell contains the indice of the trGist
%

indice = E1{ 1 ,2 };

Entrance = cell( size( indice , 1 ) , 3 );


for m = 1 : size( indice , 1 )

	tmpGist = trGist( indice{ m } , : );

	tmplabels = trlabels( indice{ m } );

    [ W1 R1 cP ] = Level2Hash( tmpGist , tmplabels , bit , method );

	% need to Calculate the J(R,1) and store the buckets
	XX = tmpGist - repmat( cP , size( tmpGist , 1 ) , 1 );
	XX = XX * W1 * R1;
	XX( XX >= 0 ) = 1;
	XX( XX <  0 ) = 0;


	[ b i j ] = unique( XX , 'rows' );

	anoymousEntrance = cell( size( b , 1 ) , 1 );

	for n = 1 : size( b , 1 )

		%Index of trGist % This code I used find and matlab ask me to use
		%the logical expression j == n to avoid vector generation?? note!

		anoymousEntrance{ n } = indice{ n }(  j == n  ); 
	end

	Entrance{ m , 1 } = [ W1 , R1 , cP ];
	Entrance{ m , 2 } = b;
	Entrance{ m , 3 } = anoymousEntrance;

end
