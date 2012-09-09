function [] = IMStest( XX , labels ,  bit , method )
% 
% demo code for generating small code and evaluation
% input XX should be a n*d matrix, n is the number of images, d is dimension
% ''method'' is the method used to generate small code
% ''method'' can be 'ITQ', 'RR', 'LSH' and 'SKLSH' 
%

X = double( XX );

% parameters
%averageNumberNeighbors = 50;    % ground truth is 50 nearest neighbor
num_test = 1000;                % 1000 query test point, rest are database
bit = bit;                      % bits used


% split up into training and test set
[ndata, D] = size(X);
R = randperm(ndata);
Xtest = X(R(1:num_test),:);
XtestLabels = labels( R(1:num_test) );

R(1:num_test) = [];
Xtraining = X(R,:);
XtrainingLabels = labels( R );
num_training = size(Xtraining,1);
clear X;

WtrueTestTraining = zeros( num_test , num_training );
for i = 1 : num_test
    for j = 1 : num_training
        if( XtestLabels( i ) == XtrainingLabels(j ) )
            WtrueTestTraining( i , j ) = 1;
        end
    end
end


% generate training ans test split and the data matrix ; ALWAYS REMEMBER
% the X is the [Xtraining ; Xtest ];
X = [Xtraining; Xtest];
% center the data, VERY IMPORTANT
sampleMean = mean(X,1);
X = (X - repmat(sampleMean,size(X,1),1));

switch( method )
    case 'CCAITQ'   
    % Convert the label information into the Y¡Ê{0,1}(nxt)
    t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!
    Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
    for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
        Y( i , XtrainingLabels(i) + 1 ) = 1;
    end

    % Apply the CCA, need to prove a little bit later , we find the W == V
    p = 0.0001;                         % follow the author in ITQ
    A = ( X(1:num_training, :)' * Y  ) / ( Y'*Y + p * eye( size( Y , 2 ) ) ) * Y' * X(1:num_training, :) ;         %for matlab no using inv...
    B = ( X(1:num_training, :)' * X(1:num_training, :) + p * eye( size( X(1:num_training, :) , 2 ) ));
    [ V D ] = eigs( A , B , bit );
    for i = 1 : size( V , 2 )           % the eigenvectors is scaled by the eigenvalues
        V( : ,i ) = V( : , i ) / sqrt( D( i , i ) );    
    end

    % now use the CCA found Wk == V to project original data
    X = X * V;

    % ITQ to find optimal rotation
    % default is 50 iterations
    % C is the output code
    % R is the rotation found by ITQ
    [C, R] = ITQ( X(1:num_training, :) , 50 );
    
    X = X*R;
    Y = zeros(size(X));
    Y(X>=0) = 1;
    Y = compactbit(Y>0);
    
    case 'CCAITQRR'
        % Convert the label information into the Y¡Ê{0,1}(nxt)
        t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!
        Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
        for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
            Y( i , XtrainingLabels(i) + 1 ) = 1;
        end

        % Apply the CCA, need to prove a little bit later , we find the W == V
        p = 0.0001;                         % follow the author in ITQ
        A = ( X(1:num_training, :)' * Y  ) / ( Y'*Y + p * eye( size( Y , 2 ) ) ) * Y' * X(1:num_training, :) ;         %for matlab no using inv...
        B = ( X(1:num_training, :)' * X(1:num_training, :) + p * eye( size( X(1:num_training, :) , 2 ) ));
        [ V D ] = eigs( A , B , bit );
        for i = 1 : size( V , 2 )           % the eigenvectors is scaled by the eigenvalues
            V( : ,i ) = V( : , i ) / sqrt( D( i , i ));    
        end

        % now use the CCA found Wk == V to project original data
        X = X * V;
    
        % RR
        R = randn(size(X,2),bit);
        [U S V] = svd(R);
        X = X*U(:,1:bit);
        Y = compactbit(X>0);
        
    case 'OURSITQ'
        % Convert the label information into the Y¡Ê{0,1}(nxt)
        t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!  
        Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
        for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
            Y( i , XtrainingLabels(i) + 1 ) = 1;
        end

        % Apply the CCA, need to prove a little bit later , we find the 
        % Wk == VC
        p = 0.0001;                         % follow the author in ITQ
        A = ( X(1:num_training, :)' * Y  ) / ( Y'*Y + p * eye( size( Y , 2 ) ) ) * Y' * X(1:num_training, :) ;         %for matlab no using inv...
        B = ( X(1:num_training, :)' * X(1:num_training, :) + p * eye( size( X(1:num_training, :) , 2 ) ));
        [ VC DI ] = eigs( A , B , bit );
        for i = 1 : size( VC , 2 )           % the eigenvectors is scaled by the eigenvalues
            VC( : ,i ) = VC( : , i ) / sqrt( DI( i , i ) );    
        end

        % now use the CCA found Wk == VC to project original data
        X = X * VC;
        V = X( 1:num_training , :);
        
        %get a rand orthogonal matrix R
        bit = size( X(1:num_training, :),2);
        R = randn(bit,bit);
        [U11 S2 V2] = svd(R);
        R = U11(:,1:bit);
        
        n_iter = 50;
        
        % ITQ find optimal rotation, but change the UX according to their
        % label
        for iter=0:n_iter
            Z = V * R;      
            UX = ones(size(Z,1),size(Z,2)).*-1;
            UX(Z>=0) = 1;                         
            %Now the UX is the new B(Y) , we need to change it!
            for i = 1 : size( Y , 2 )
                index =  find( XtrainingLabels == ( i-1 ) );   %handle this class label
                UI = UX( index , : );
                UI( UI<=0 ) = 0;
                num = zeros( size( UI , 1 ) , 1 );
                for l = 1 : size( UI , 2 )
                    num( : ) = num( : ) * 2 + UI( : , l );
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
        Q = eye( size( V,1 ) );              %q is diag dxd matrix
        Q = ( q^2 / 3 ) * Q ;
        A1 = VC' * (XX(1:num_training,:))' * XX(1:num_training,:) * VC;
        A2 = VC' * Q * VC ;
        S = ( A1 + A1' + A2 + A2' ) \ ( R * Y' * XX(1:num_training,:) * VC )';  %omitting (RR')-1 ||  %not to use inv?? || RR' must be eye right?
        
        XX = XX * V ;
        XX = XX*R;
        XX = XX*S;
        
        Y = zeros(size(XX));
        Y(XX>=0) = 1;
        
        Y = compactbit(Y>0);
        
end

% compute Hamming metric and compute recall precision
B1 = Y(1:size(Xtraining,1),:);
B2 = Y(size(Xtraining,1)+1:end,:);
Dhamm = hammingDist(B2, B1);
[recall, precision, rate] = recall_precision(WtrueTestTraining, Dhamm);

% plot the curve
switch(method)
    % ITQ method proposed in our CVPR11 paper
    case 'CCAITQ'
    plot(recall,precision,'-o');
    case 'CCAITQRR'
    plot(recall,precision,'-s');
    case 'OURSITQ'
    plot(recall,precision,'-X');
end

xlabel('Recall');
ylabel('Precision');




