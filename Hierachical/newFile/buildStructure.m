function [ ] = buildStructure( trGist , trLabels , secondBit , thirdBit , E1 , method )
%
%  This function will use the E1 and build all the rest structure in disk
%  
%  IMS@SCUT
%  2012/11/13

ent = E1{ 1 , 3 };

for a = 1 : size( ent , 1 )
    index = ent{ a };
    
    samples = trGist( index , : );
    labels = trLabels( index );
    
    E2 = getEntrance1( samples , labels , secondBit , method );
    
    indice = E2{ 1 ,3 };

    E3 = cell( size( indice , 1 ) , 3 );

    for m = 1 : size( indice , 1 )

        tmpGist = samples( indice{ m } , : );

        tmplabels = labels( indice{ m } );

        if size( tmplabels , 1 ) > 1  
        
            [ W1 R1 cP ] = Level2Hash( tmpGist , tmplabels , thirdBit , method );

            XX = tmpGist - repmat( cP , size( tmpGist , 1 ) , 1 );
            XX = XX * W1 * R1;
            XX( XX >= 0 ) = 1;
            XX( XX <  0 ) = 0;


            [ b i j ] = unique( XX , 'rows' );

            anoymousEntrance = cell( size( b , 1 ) , 1 );

            for n = 1 : size( b , 1 )
                anoymousEntrance{ n } = indice{ m }(  j == n  );    % final level we store index of the trGist
                anoymousEntrance{ n } = index( anoymousEntrance{ n } ) ;
            end

            E3{ m , 1 } = { W1 , R1 , cP };
            E3{ m , 2 } = b;
            E3{ m , 3 } = anoymousEntrance;
        
        else
            % when the third level only have one image , we have to handle
            % it like this
            cP = zeros( 1 , size( tmpGist , 2 ) );
            W1 = zeros( size( tmpGist , 2 ) , thirdBit );
            R1 = zeros( thirdBit );
            
            b = zeros( 1 , thirdBit );
            
            anoymousEntrance = cell( 1 , 1 );
            anoymousEntrance{ 1, 1 } = tmplabels;
            
            E3{ m , 1 } = { W1 , R1 , cP };
            E3{ m , 2 } = b;
            E3{ m , 3 } = anoymousEntrance;
            
        end
        
    end

    fileName = [ 'E' num2str( a ) ];
    
    E2{ 1 , 3 } = [];
    save( fileName ,  'E2' , 'E3' );
    clear E2 E3
    
end





