function [] = IMStest( XX , labels ,  bit , method )
% 
% demo code for generating small code and evaluation
% input XX should be a n*d matrix, n is the number of images, d is dimension
% ''method'' is the method used to generate small code
% ''method'' can be 'ITQ', 'RR', 'LSH' and 'SKLSH' 
%

XX = double( XX );
X  = XX;

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
    [ W D ] = eigs( A , B , bit );
    for i = 1 : size( W , 2 )           % the eigenvectors is scaled by the eigenvalues
        W( : ,i ) = W( : , i ) / D( i , i );    
    end

    % now use the CCA found Wk to project original data
    X = X * W;

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
        [ W D ] = eigs( A , B , bit );
        for i = 1 : size( W , 2 )           % the eigenvectors is scaled by the eigenvalues
            W( : ,i ) = W( : , i ) / D( i , i );    
        end

        % now use the CCA found Wk == V to project original data
        X = X * W;
    
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
        % Wk
        p = 0.0001;                         % follow the author in ITQ
        A = ( X(1:num_training, :)' * Y  ) / ( Y'*Y + p * eye( size( Y , 2 ) ) ) * Y' * X(1:num_training, :) ;         %for matlab no using inv...
        B = ( X(1:num_training, :)' * X(1:num_training, :) + p * eye( size( X(1:num_training, :) , 2 ) ));
        [ W D ] = eigs( A , B , bit );
        for i = 1 : size( D , 2 )           % the eigenvectors is scaled by the eigenvalues
            W( : ,i ) = W( : , i ) / D( i , i );    
        end

        % now use the CCA found Wk == VC to project original data      
        n_iter = 50;
        [B R S ] = OURSITQ( X( 1:num_training , : ) , W , XtrainingLabels , n_iter );
        
        X = X * W * R * S;
        Y = zeros(size(X));
        Y(X>=0) = 1;
        
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




