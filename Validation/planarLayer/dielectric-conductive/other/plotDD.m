% Simulation settings for a domain ((-0.5,-0.5)--(0.5,0.5))
% with a centered charge bump
L          = 1;
phiMax     = 1;
eps1       = 3e-11;
eps2       = 1e-11;
beta       = eps1/eps2;
timeToLoad = '2e-08';

% Load simulation solution
% Load the file from the sample utility
fname=['sets/' timeToLoad '/lineX1_Ue_alpha1_magGradUe_p.xy'];
sim = importdata(fname);
y         = sim(:,1);
phase     = sim(:,3);

% Exact solutions for the dielectric-dielectric case
ex.phi1   = (-2*y+beta)/(1+beta);
ex.E1     = 2/(1+beta);
ex.phi2   = (beta*(-2*y+1))/(1+beta);
ex.E2     = (2*beta)/(1+beta);
ex.deltaP = -(2*beta*(beta-1))/((1+beta)^2);
ex.phi    = phase.*ex.phi1 + (1-phase).*ex.phi2;


% Plot the electric potential simulation data and analytical solution
figure; hold on; box on;
plot(y, ex.phi, '-');
plot(y, sim(:,2), 'o');
title('Electric potential vs. position');
xlabel('Position y [m]');
ylabel('Electric potential Ue [V]');

% Check electric field strength and pressure drop

% Normalize the pressure drop with eps2*phiMax^2/L^2 for both phases
P1 = mean(sim(find(phase==1),5))/(eps2*phiMax^2/L^2);
P2 = mean(sim(find(phase==0),5))/(eps2*phiMax^2/L^2);
deltaP = P1-P2;
