function [B,R,S] = OURSITQ( V , Labels , n_iter )
%
% main function for OURSITQ which finds a rotation of the CCA projected data
%
% Input:
%       V: n*c CCA projected data, n is the number of images and c is the
%       code length
%       n_iter: max number of iterations, 50 is usually enough
% Output:
%       B: n*c binary matrix
%       R: the c*c rotation matrix found by OURSITQ
% Author:
%       IMS@SCUT

% initialize with a orthogonal random rotation
bit = size(V,2);
R = randn(bit,bit);
[U11 S2 V2] = svd(R);
R = U11(:,1:bit);

% use ITQ to find optimal rotation, but change the UX according to their
% label
for iter=0:n_iter
    Z = V * R;      
    UX = ones(size(Z,1),size(Z,2)).*-1;
    UX(Z>=0) = 1;                         
    %Now the UX is the new B(Y) , we need to change it!
    for i = 1 : size( Labels , 2 )
        index =  find( Labels == ( i-1 ) );   %handle this class label
        UI = UX( index , : );
        UI( UI<0 ) = 0;
        num = zeros( size( UI , 1 ) , 1 );
        for k = 1 : size( UI , 2 )
            num( : ) = num * 2 + UI( : , k );
        end
        [ a b ] = mode( num );
        num_i = find( num == a );
        UX( index , : ) = repmat( UX( num_i(1) , : ) , length( index ) , 1 );
    end
            
     %done changing the UX to what we want....                                    
     C = UX' * V;
     [UB,sigma,UA] = svd(C);    
     R = UA * UB';
     end
        
     Y = UX;
     %Now the R is found! Find the S
     q = 10;                               %q is fro [-q,q] user define boundary
     Q = eye( size( VC,1 ) );              %q is diag dxd matrix
     Q = ( q^2 / 3 ) * Q ;
     A1 = VC' * (XX(1:num_training,:))' * XX(1:num_training,:) * VC;
     A2 = VC' * Q * VC ;
     S = ( A1 + A1' + A2 + A2' ) \ ( R * Y' * XX(1:num_training,:) * VC )';  %omitting (RR')-1 ||  %not to use inv?? || RR' must be eye right?
        
     X = X*R;
     X = X*S;

% make B binary
B = UX;
B(B<0) = 0;









