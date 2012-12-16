classdef Tracker
    properties  % the data
       tkType   %a string, what kind of tracker? sphere ellipsoid cylinder or Multi Hyper tracker?
       
       %Location of Previous
       Location  %row and col
       %shift, move speed
       speed     %the different of previous location and new location
       
       %sphere
       CenterOfSphere
       Radius
       
       %ellipsoid
       CenterOfEllipsoid  % the ( x - v )' * ivA * ( x - v ) = d, this is v
       ivA                % the ( x - v )' * ivA * ( x - v ) = d, 
       d                  % the ( x - v )' * ivA * ( x - v ) = d
       
       %cylinder
       CenterPoint
       DirOfA
       la
       dc
       
    end

    methods
       function [ boool ] = isTargetColor( tk , x ) %ask if the point x [ r g b ]' support the objcet
           tk.tkType
           boool = 1;
       end
       
       %
       function [ tk ] = constructHelp( tk , varargin )
           data = varargin{1}.data;
           tk.Location = varargin{1}.location;
           
           switch tk.tkType
               case 'sphere'
                   %varargin{ 2 } may be : 'a' 'b' 'c' 'avg' preOfavg 
                   %find a proper sphere to fit the color!
                   %how to filter out some bad RGP points, it's too noisy
                   
                   tk.CenterOfSphere = mean( data )';  %find the center
                   
                   cm = cov( data ); %find the covariance matrix

                   [ P , Lam ] = eigs( cm ); %eigenvector eigenvalue

                   poj = data * P(:,1);
                   a = max( poj ) - min( poj );
                   a = a / 2;                %a is the longest axis

                   poj = data * P(:,2);
                   b = max( poj ) - min( poj );
                   b = b / 2;                %b is the second longest axis

                   poj = data * P(:,3);
                   c = max( poj ) - min( poj );
                   c = c / 2;                %c is the shortest axis
                   
                   if isa( varargin{1} , 'char' )
                       switch varargin{2}
                           case 'a'
                               tk.Radius = a;
                           case 'b'
                               tk.Radius = b;
                           case 'c'
                               tk.Radius = c;
                           case 'avg'  %vector operation is much faster in matlab
                               P1 = data;
                               P2 = repmat( tk.CenterOfSphere' ,  size( P1 , 1 ) , 1 );
                               R = P1 * tk.CenterOfSphere;
                               dist =sum( P1.^2 + P2.^2 , 2 ) - 2 * R;
                               dist =sqrt( dist );
                               
                               tk.Radius = mean( dist );
                               
                           otherwise
                               error('constructHelp: Invalid Argument');
                       end
                      
                   else
                       P1 = data;
                       P2 = repmat( tk.CenterOfSphere' ,  size( P1 , 1 ) , 1 );
                       R = P1 * tk.CenterOfSphere;
                       dist =sum( P1.^2 + P2.^2 , 2 ) - 2 * R;
                       dist =sqrt( dist );
                               
                       tk.Radius = mean( dist ) * varargin{2} ;
                       
                   end
                   
                                      
               case 'ellipsoid'
                   %varargin{ 2 3 4 5} should be : scale of a b c, and di
                   %find a proper ellipsoid to fit the color!
                   %how to filter out some bad RGP points, it's too noisy
                   sa = varargin{2};
                   sb = varargin{3};
                   sc = varargin{4};
                   di  = varargin{5};
                   
                   
                   cp = mean( data );
                   tk.CenterOfEllipsoid = cp';

                   cm = cov( data ); %find the covariance matrix

                   [ P , Lam ] = eigs( cm ); %eigenvector eigenvalue

                   poj = data * P(:,1);
                   a = max( poj ) - min( poj );
                   a = a / 2;
                   a = a * sa;

                   poj = data * P(:,2);
                   b = max( poj ) - min( poj );
                   b = b / 2;
                   b = b * sb;

                   poj = data * P(:,3);
                   c = max( poj ) - min( poj );
                   c = c / 2;
                   c = c * sc;

                   a2 = a * a;
                   b2 = b * b;
                   c2 = c * c;

                   B = diag( [ a2 b2 c2 ] );

                   A = P * B * inv( P );

                   tk.rvA = inv( A );   % ( x - cp )' * ivA * ( x - cp ) = 1 or > 1 or < 1
                   tk.d = di;
                       
               case 'cylinder'
                   %varargin{ 2 3} should be : scale of a, di
                   %find a proper ellipsoid to fit the color!
                   %how to filter out some bad RGP points, it's too noisy
                   sa = varargin{2};
                   di  = varargin{3};
                   
                   cp = mean( data );
                   tk.CenterPoint= cp';

                   cm = cov( data ); %find the covariance matrix

                   [ P , Lam ] = eigs( cm ); %eigenvector eigenvalue
                   
                   poj = data * P(:,1);
                   a = max( poj ) - min( poj );
                   a = a / 2;
                   a = a * sa;
                   
                   tk.DirOfA = P(:,1);
                   tk.la = a;
                   tk.dc = di;
                   
           end
           
       end %end of fucntion constructHelp
       
       %
       function [ ] = showBoundary( tk )
           switch tk.tkType
               case 'sphere'
                   
               case 'ellipsoid'
                   
               case 'cylinder'
                   
           end
                 
       end %end of showType
       
       %
       function tk = Tracker( varargin )

        % Tracker constructor function
        % tk = Tracker; %get a object of Class Tracker
        % tk = Tracker( b ); %useing parameter b to get object
        %           
        %

        switch nargin
            case 0
                error('Tracker: Wrong Argument, No RGP Points !!');
                %tk.tkType = 'ellipsoid';
                %tk = class( tk , 'Tracker' );
                
            otherwise
                if isa( varargin{1} , 'char' )
                    method = varargin{1};
                    
                    switch method
                        case 'sphere'
                            
                        case 'ellipsoid'
                            
                        case 'cylinder'
                            
                        case 'Multi'
                            
                        case 'Hyper'
                            
                        otherwise
                            error('Tracker: Invalid type of Tracker');
                    end
                    
                    tk.tkType = method;
                    tk = constructHelp( tk , varargin );
                    %tk = class( tk , 'Tracker' ); %here tk is already a
                    %class
                else
                    error('Tracker : Wrong argument type' );
                end
        end
       end %end of Tracker
        
    end
end
