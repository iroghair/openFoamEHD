/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  1.5                                   |
|   \\  /    A nd           | Web:      http://www.OpenFOAM.org               |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    object      blockMeshDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

convertToMeters 1;

vertices        
(
    (-0.5 -0.5 -0.01)
    (0.5 -0.5 -0.01)
    (0.5 0.5 -0.01)
    (-0.5 0.5 -0.01)
    (-0.5 -0.5 0.01)
    (0.5 -0.5 0.01)
    (0.5 0.5 0.01)
    (-0.5 0.5 0.01)
);

blocks          
(
    hex (0 1 2 3 4 5 6 7) (RES RES 1) simpleGrading (1 1 1)
);

edges           
(
);

patches         
(
    wall upWall 
    (
        (3 7 6 2)
    )
    wall downWall 
    (
        (1 5 4 0)
    )
    wall leftandright 
    (
        (0 4 7 3)
        (6 5 1 2)
    )
    empty frontAndBack 
    (
        (0 3 2 1)
        (6 7 4 5)
    )
);

mergePatchPairs 
(
);

// ************************************************************************* //
