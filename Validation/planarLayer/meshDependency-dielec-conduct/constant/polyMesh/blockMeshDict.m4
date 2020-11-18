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
    (-0.005   -0.5 -0.01)
    ( 0.005 -0.5 -0.01)
    ( 0.005  0.5 -0.01)
    (-0.005  0.5 -0.01)
    (-0.005 -0.5  0.01)
    ( 0.005 -0.5  0.01)
    ( 0.005  0.5  0.01)
    (-0.005  0.5  0.01)

    (-0.005 0 -0.01)
    ( 0.005 0 -0.01)
    (-0.005 0  0.01)
    ( 0.005 0  0.01)

);

blocks          
(
    hex (0 1 9 8 4 5 11 10) (6 YRES 1) simpleGrading (1 1 1)
    hex (8 9 2 3 10 11 6 7) (6 YRES 1) simpleGrading (1 1 1)
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
        (0 4 10 8)
        (8 10 7 3)
        (2 6 11 9)
        (9 11 5 1)
    )
    empty frontAndBack 
    (
        (0 8 9 1)
        (8 3 2 9)
        (4 5 11 10)
        (10 11 6 7)
    )
);

mergePatchPairs 
(
);

// ************************************************************************* //
