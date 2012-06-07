function [ DiscreData,DiscretizationSet ] = CACC_Discretization( OriginalData, C )
%Paper: Cheng-Jung Tsai , Chien-I. Lee , Wei-Pang Yang, A discretization
%algorithm based on Class-Attribute Contingency Coefficient, Information Sciences: an International Journal, v.178 n.3, p.714-731, February, 2008 

%1 Input: Dataset with i continuous attribute, M examples and S target classes;
%2 Begin
%3 For each continuous attribute Ai
%4 Find the maximum dn and the minimum d0 values of Ai;
%5 Form a set of all distinct values of A in ascending order;
%6 Initialize all possible interval boundaries B with the minimum and maximum
%7 Calculate the midpoints of all the adjacent pairs in the set;
%8 Set the initial discretization scheme as D: {[d0,dn]}and Globalcacc = 0;
%9 Initialize k = 1;
%10 For each inner boundary B which is not already in scheme D,
%11 Add it into D;
%12 Calculate the corresponding cacc value;
%13 Pick up the scheme D?with the highest cacc value;
%14 If cacc > Globalcacc or k < S then
%15 Replace D with D?
%16 Globalcacc = cacc;
%17 k = k + 1;
%18 Goto Line 10;
%18 Else
%19 D?= D;
%20 End If
%21 Output the Discretization scheme D?with k intervals for continuous attribute Ai;
%22 End

% This code is implemented by Guangdi Li, 2009/06/04

% OriginalData is organized as F1,F2,...,Fm,C1,C2,...,Cn
F = size( OriginalData,2 ) - C ;
M = size( OriginalData,1 );
DiscreData = zeros( M,C+F ); 
DiscreData( :,F+1:F+C ) = OriginalData( :,F+1:F+C );
% Assume the maximum number of interval is M/(3*C)
MaxNumF = floor(M/(3*C));
% Save all the discretization intervals, which is saved in column
DiscretizationSet = zeros( MaxNumF,F );

for p = 1:F
    % Step 1
    %Dn = max( OriginalData( :,p )); % the maximum boundary 
    %Do = min( OriginalData( :,p )); % the minimum boundary   
    SortedInterval = unique( OriginalData( :,p ) );
    if length(SortedInterval) == 1 % all values are equal
        DiscretizationSet( 1,p )= SortedInterval;
        DiscreData( :,p ) = zeros(M,1);  
        continue;
    end
        
    B = zeros( 1,length( SortedInterval )-1 );
    Len = length( B );
    for q = 1:Len
        B( q ) = ( SortedInterval( q ) + SortedInterval( q+1 ) )/ 2;
    end
    %B  
    D = zeros( 1,MaxNumF ); % D save all discretizations for variable Fi
    %D( 1 ) = Do; D( 2 ) = Dn; 
    GlobalCACC = -Inf;
    %B
    %p
    %Step 2
    k=0; % save the number of discretizations in D, the initiate state is 2 
    while true
          CACC = - Inf; Local = 0;
          for q = 1:Len
              if isempty( find( D( 1:k )==B(q), 1 ) ) == 1                  
                 DTemp = D;
                 DTemp( k+1 ) = B( q );
                 DTemp( 1:( k+1 ) ) = sort( DTemp( 1:( k+1 ) ) );
                 CACCValue = CACC_Evaluation( OriginalData,C,p,DTemp( 1:( k+1 ) ) );
                 if CACC < CACCValue 
                    CACC = CACCValue;
                    Local= q;
                 end           
              end        
          end
          %Local
          %CACC
          %GlobalCACC
          if GlobalCACC < CACC && k < MaxNumF 
             GlobalCACC = CACC
             k = k + 1;
             D( k ) = B( Local );
             D( 1:k ) = sort( D( 1:k ) );
          elseif  k <= MaxNumF && k <= C && Local ~= 0
             k = k + 1;
             D( k ) = B( Local );
             D( 1:k ) = sort( D( 1:k ) );              
          else
              break;
          end   
    end
    DiscretizationSet( 1:k,p )= D( 1:k )';
    % do the discretization process according to intervals in D. 
    DiscreData( :,p ) = DiscretWithInterval( OriginalData,C,p,D( 1:k ) );    
end
end

function CACCValue = CACC_Evaluation( OriginalData, C, Feature, DiscretInterval )
%Paper: Kurgan, L. and Cios, K.J. (2002). CAIM Discretization Algorithm, IEEE Transactions of Knowledge and Data Engineering, 16(2): 145-153
% OriginalData is organized as F1,F2,...,Fm,C1,C2,...,Cn

M = size( OriginalData,1 );
k = length( DiscretInterval );
[ DiscretData,QuantaMatrix ] = DiscretWithInterval( OriginalData,C,Feature,DiscretInterval );
%Discrete the continuous data upon OriginalData 

%QuantaMatrix
% Compute the value of CAIM via quanta matrix and equation (sum maxr/Mr)/n 

RowQuantaMatrix = sum( QuantaMatrix,2 );
ColumnQuantaMatrix = sum( QuantaMatrix,1 );
CACCValue = 0 ;

for p = 1:C
    for q = 1:k
       if RowQuantaMatrix( p ) > 0 && ColumnQuantaMatrix( q ) > 0
          CACCValue = CACCValue + ( QuantaMatrix( p,q ) )^2/( RowQuantaMatrix( p )*ColumnQuantaMatrix( q )) ;
       end
    end
end
CACCValue = M*( CACCValue-1 )/log2(k+1) ;
end

function [ DiscretData,QuantaMatrix ] = DiscretWithInterval( OriginalData,C,Column,DiscretInterval )
% C is the number of class variables.

M = size( OriginalData,1 );
k = length( DiscretInterval );
F = size( OriginalData,2 ) - C;
DiscretData = zeros( M,1 );
%Discrete the continuous data upon OriginalData 
for p = 1:M
    for t = 1:k
         if OriginalData( p,Column ) <= DiscretInterval( t )
             DiscretData( p ) = t-1;
             break;
         elseif OriginalData( p,Column ) > DiscretInterval( k )
             DiscretData( p ) = k;
         end             
     end        
end

%OriginalData( :,Column )
%Quanta matrix 
CState = C;
FState = length( DiscretInterval ) + 1;
QuantaMatrix = zeros( CState,FState );
for p = 1:M
    for q = 1:C
        if OriginalData( p,F+q ) == 1
           Row = q;
           Column = DiscretData( p )+1;
           QuantaMatrix( Row,Column ) = QuantaMatrix( Row,Column ) + 1;
        end
    end
end
%QuantaMatrix
end