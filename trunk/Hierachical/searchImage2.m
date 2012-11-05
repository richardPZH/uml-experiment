function [ recall precision ] = searchImage2( inGist , inLabel , inVector , trGist , trLabels , trVector , E1 , E2 , E3 )
%
% This function use the testing image to evaluate the performance 
% of the Hierachical hashing method... 
%
% Input:
%     inGist, nx320 CIFAR Gist image representation, n images to be evaluate
%	  inLabel, nx1, ground true label of the image; 
%     inVector, the nx3072 original image, can use to display images to human
%     trGist, mx320 training Gist image representation
%     trLabels, 
%     trVector,
%     E1, the first level entrance
%     E2, the second level entrance
%     E3, the third level entrance
%
% Output:
%     recall: average recall of all the n images
%     precision: average precision of ...
%
% Authors:
%     IMS@SCUT Once 2012/11/04
%
% Advices:
%     How to use more vector operation rather than for loop


recall = 0;
precision = 1;

% the hamming distance in level 1, which we need to perdefine
L1Dis = 2;

%get the first level hash code
W0=  E1{ 1 , 1 }{ 1 };
R0 = E1{ 1 , 1 }{ 2 };
cP0 = E1{ 1 , 1 }{ 3 };

% first level hash
XX = inGist - repmat( cP0 , size( inGist , 1 ) , 1 );

XX = XX * W0 * R0;

XX( XX >= 0 ) = 1;
XX( XX <  0 ) = 0;

% In the Entrace 1 , E1{1,2} is the binary code matrix, one row, one group images
L1Code = E1{ 1 , 2 };

for numOfImage = 1 : size( inLabel , 1 )
   
    imageGist = inGist( numOfImage , : );     %search image in gist
    imageL1Code = XX( numOfImage , : );       %search image 1st code
    imageVector = inVector( numOfImage , : ); %search image in RGB
    imageLabel = inLabel( numOfImage );       %search image label
    
    %search one image starts
    hmd = CalHammingDist( imageL1Code , L1Code , 'vec' );
    
    for dist = 0 : L1Dis 
        % handle distance of dist starts
        vec = find( hmd == dist );
        
        for a = 1 : size( vec , 1 )
            
            [ recall precision ] = Level2Search( recall , precision , imageGist , imageVector , imageLabel , trGist , trLabels , trVector , vec( a ) , E2 , E3 );
            
        end
        
        % handle distance of dist ends
    end
    
    
    
    %search one image finishes
    
end

