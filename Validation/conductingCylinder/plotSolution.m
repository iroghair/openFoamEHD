% Simulation settings for a domain ((-1,-1)--(1,1))
% with a centered conducting cylinder and initial charge

R          = 0.05;          % Radius of cylinder
totRhoE    = 2.83334e-05;   % Total free charge in the domain (hint: use foamCalcEx or 'integrate variables' filter in paraFoam to obtain this number)
D          = 0.02;          % Depth of 2D domain
eps2       = 2;             % Pemittivity of dielectric
timeToLoad = '30';          % Simulation time to load

Q          = totRhoE/D;     % Charge per unit length of cylinder

% Load simulation solution
% Load the file from the sample utility
fname=['sets/' timeToLoad '/lineX1_Ue_alpha1_magGradUe_p.xy'];
sim = importdata(fname);
pos       = find(sim(:,1)>=0);
x         = sim(pos,1);
EE        = sim(pos,4);
exact = zeros(length(x),1)
% Exact solution
for i = 1:length(x)
    if (x(i) < R)
        exact(i) = 0;
    else
        exact(i) = Q/(2*pi*eps2).*(1/x(i));
    end
end

% Plot the electric potential simulation data and analytical solution
figure; hold on; box on;
plot(x/R, exact, '-');
plot(x/R, EE, 'o');
title('Electric field strength vs. position');
xlabel('Position r/R [-]');
ylabel('Electric field strength [V/m]');

