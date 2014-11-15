// Original code by inigo quilez - iq/2014
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

uniform sampler2D iChannel0;
uniform vec3  iResolution;
uniform float iGlobalTime;
uniform float camAngle;

varying vec3  iPosition;

#define PI 3.1415926535897932384626433832795

const float halfPI = 0.5 *PI;
const float qtr_PI = 0.25*PI;
const mat2 m2 = mat2(1.6,-1.2,1.2,1.6);

float terrain( vec2 p, int j ) {
    p.x *= iResolution.x/iResolution.y;
    //

    p *= 0.0013;
    float s = 1.0;
    float t = 0.0;
    for ( int i=0; i<j; i++ ) {
        t += 0.5*(cos(6.2831*p.x) + sin(6.2831*p.y))*s;
        s *= 0.5 + 0.1*t;
        p = 0.97*m2*p + (t-0.5)*0.2;
    }
    return t*55.0;
}

float map( in vec3 pos ) {
    return pos.y - terrain(pos.xz, 4);
}

float intersect( in vec3 ro, in vec3 rd, in float tmin, in float tmax, in float time ) {
    float t = tmin;
    for ( int i=0; i<160; i++ ) {
        vec3 pos = ro + t*rd;
        float h = map( pos );
        if ( h < (0.001*t) || t>tmax ) break;
        t += h * 0.5;
    }
    return t;
}

float calcShadow(in vec3 ro, in vec3 rd ) {
    vec2 eps = vec2( 150.0, 0.0 );
    float h1 = terrain( ro.xz, 4 );
    float h2 = terrain( ro.xz, 2 );
    float d1 =  10.0;
    float d2 =  80.0;
    float d3 = 200.0;
    float s1 = clamp( 1.0*(h1 + rd.y*d1 - terrain(ro.xz + d1*rd.xz, 4)), 0.0, 1.0 );
    float s2 = clamp( 0.5*(h1 + rd.y*d2 - terrain(ro.xz + d2*rd.xz, 4)), 0.0, 1.0 );
    float s3 = clamp( 0.2*(h2 + rd.y*d3 - terrain(ro.xz + d3*rd.xz, 2)), 0.0, 1.0 );
    return min(min(s1,s2),s3);
}

vec3 calcNormal( in vec3 pos, float t, int j ) {
    float e = 0.005*t;
    vec2 eps = vec2( e, 0.0 );
    float h = terrain( pos.xz, j );
    return normalize( vec3( terrain(pos.xz-eps.xy,j)-h, e, terrain(pos.xz-eps.yx,j)-h ));
}

vec3 camPath( float time ) {
    vec2 p = 1200.0*vec2( cos(0.0+0.23*time), cos(1.5+0.205*time) );
    return vec3( p.x, 0.0, p.y );
}

vec3 dome( in vec3 rd, in vec3 light1 ) {
    float sda = clamp(0.5 + 0.5*dot(rd,light1),0.0,1.0);
    float cho = max(rd.y,0.0);
    vec3 bgcol = mix( mix(vec3(0.00,0.40,0.60)*0.7, 
                          vec3(0.80,0.70,0.20),                        pow(1.0-cho,3.0 + 4.0-4.0*sda)), 
                          vec3(0.43+0.2*sda,0.4-0.1*sda,0.4-0.25*sda), pow(1.0-cho,10.0+ 8.0-8.0*sda) );
    bgcol *= 0.8 + 0.2*sda;
    return bgcol*0.75;
}

void main( void ) {
    vec2 xy = -1.0+2.0*gl_FragCoord.xy/iResolution.xy;
    vec2 sp = xy*vec2(iResolution.x/iResolution.y,1.0);

    // animate    
    float time = 16.5 + (0.0+iGlobalTime-0.0)*0.1;

    // ray origin
    vec3  ro = camPath( time + 0.0 );
    ro.y = terrain( ro.xz, 7 ) + 24.0;

    // target
    vec3  ta = camPath( time + 2.0 );
    ta.y = ro.y - 180.0;

    // enable camera pivot
    //ta.x = cos(camAngle)*200.0 + ro.x;
    //ta.z = sin(camAngle)*200.0 + ro.z;

    vec3 light1 = normalize( vec3(-0.8,0.2,0.5) );

    // calc a distant tangent point
    //vec3 tmp = vec3(0.0,0.0,0.0)-ro;
    //vec3 tc = vec3(tmp.x*cos(halfPI)+tmp.z*sin(halfPI), tmp.y, tmp.z*cos(halfPI)-tmp.x*sin(halfPI));
    //tc = ro+tc;

    // calc control points
    vec3 dv = ta-ro;
    vec3 tmp = vec3(dv.x*cos(-halfPI)+dv.z*sin(-halfPI), dv.y, dv.z*cos(-halfPI)-dv.x*sin(-halfPI));
    vec3 cpr = ro + normalize(dv)*4.0;
    cpr.y = terrain( cpr.xz, 7 );
    if ( cpr.y < ro.y ) {
        // downward slope
        cpr *= vec3(1.0, 0.25*(ro.y-cpr.y), 1.0);
    } else {
        // upward slope
        cpr *= vec3(1.0, 0.25*(cpr.y-ro.y), 1.0);
    }

    //vec3 cpl = ro + vec3(dv.x*cos( qtr_PI)+dv.z*sin( qtr_PI), dv.y, dv.z*cos( qtr_PI)-dv.x*sin( qtr_PI));
    //cpl.y = terrain( cpl.xz, 4 );
    //

    // camera
    vec3  cw = normalize(ta-ro);                 // forward direction
    vec3  cp = normalize( cross(cpr,cw) );       // roll  (rot on Z-axis)
    vec3  cu = normalize( cross(cw,cp ) );       // pitch (rot on X-axis)

    //vec3  cv = normalize( cross(cu,cw) );       // yaw   (rot on Y-axis)

    // ray direction
    vec3  rd = normalize( sp.x*cu + sp.y*cp + 1.5*cw );
    
    // background    
    vec3 bgcol = dome( rd, light1 );

    // raymarch
    float tmin = 10.0;
    float tmax = 2000.0;

    float sundotc = clamp( dot(rd,light1), 0.0, 1.0 );
    vec3 col = bgcol;

    float t = intersect( ro, rd, tmin, tmax, time );
    if ( t>tmax ) {
        // sky      
        col += 0.2*0.12*vec3(1.0,0.5,0.1)*pow( sundotc,5.0 );
        col += 0.2*0.12*vec3(1.0,0.6,0.1)*pow( sundotc,64.0 );
        col += 0.2*0.12*vec3(2.0,0.4,0.1)*pow( sundotc,512.0 );

        // clouds
        vec2 sc = ro.xz + rd.xz*(1000.0-ro.y)/rd.y;
        col = mix( col, 0.25*vec3(0.5,0.9,1.0), 0.4*smoothstep(0.0,1.0,texture2D(iChannel0,0.000005*sc).x) );

        // sun scatter
        col += 0.2*0.2*vec3(1.5,0.7,0.4)*pow( sundotc, 4.0 );
    }
    else {
        // mountains        
        vec3 pos = ro + t*rd;
        vec3 nor = calcNormal( pos, t, 5 );
        vec3 sor = calcNormal( pos, t, 2 );
        vec3 ref = reflect( rd, nor );

        // rock
        col = vec3(0.07,0.06,0.05);
        col *= 0.2 + sqrt( texture2D( iChannel0, 0.01*pos.xz*vec2(0.5,1.0) ).x *
                           texture2D( iChannel0, 0.01*pos.xz*vec2(0.5,1.0) ).x );
        vec3 col2 = vec3(1.0,0.2,0.1)*0.01;
        col = mix( col, col2, 0.0 );
        
        // grass
        float s = smoothstep(0.6,0.7,nor.y - 0.01*(pos.y-20.0));        
        s *= smoothstep( 0.15,0.2,0.01*nor.x+texture2D(iChannel0, 0.001*pos.xz).x);
        vec3 gcol = 0.13*vec3(0.22,0.23,0.04);
        gcol *= 0.3+texture2D( iChannel0, 0.03*pos.xz ).x*1.4;
        col = mix( col, gcol, s );
        nor = mix( nor, sor, 0.3*s );
        vec3 ptnor = nor;

        // trees
        s = smoothstep(0.9,0.95,nor.y - 0.01*(pos.y-20.0));        
        s *= smoothstep( 0.1,0.13,-0.17+texture2D(iChannel0, 0.0025*pos.xz).x);
        vec3 tor = -1.0 + 2.0*texture2D( iChannel0, 0.015*pos.xz ).xyz;
        tor.y = 1.5;
        tor = normalize(tor);
        col = mix( col, 0.11*vec3(0.22,0.25,0.02)*1.0, s );
        nor = mix( nor, tor, 0.7*s );
        
        // snow
        s = ptnor.y + 0.008*pos.y - 0.2 + 0.2*(texture2D(iChannel0,0.00015*pos.xz+0.0*sor.y).x-0.5);
        float sf = fwidth(s) * 1.5;
        s = smoothstep(0.84-sf, 0.84+sf, s );
        col = mix( col, 0.15*vec3(0.42,0.6,0.8), s);
        nor = mix( nor, sor, 0.5*smoothstep(0.9, 0.95, s ) );

        // lighting     
        float amb = clamp( nor.y,0.0,1.0);
        float dif = clamp( dot( light1, nor ), 0.0, 1.0 );
        float bac = clamp( dot( normalize( vec3(-light1.x, 0.0, light1.z ) ), nor ), 0.0, 1.0 );
        float sha = mix( calcShadow( pos, light1 ), 1.0, 0.0 );
        
        vec3 lin  = vec3(0.0);
        lin += dif*vec3(11.0,6.00,3.00)*vec3( sha, sha*sha*0.5+0.5*sha, sha*sha*0.8+0.2*sha );
        lin += amb*vec3(0.25,0.30,0.40);
        lin += bac*vec3(0.35,0.40,0.50);
        col *= lin;

        // fog
        col = mix( col, 0.25*mix(vec3(0.4,0.75,1.0),vec3(0.3,0.3,0.3), sundotc*sundotc), 1.0-exp(-0.0000008*t*t) );

        // sun scatter
        col += 0.15*vec3(1.0,0.8,0.3)*pow( sundotc, 8.0 )*(1.0-exp(-0.003*t));

        // background
        col = mix( col, bgcol, 1.0-exp(-0.00000004*t*t) );
    }

    // gamma
    col = pow( col, vec3(0.45) );

    // color grading    
    col = col*1.4*vec3(1.0,1.0,1.02) + vec3(0.0,0.0,0.11);
    col = clamp(col,0.0,1.0);
    col = col*col*(3.0-2.0*col);
    col = mix( col, vec3(dot(col,vec3(0.333))), 0.25 );

    // panoramic viewport
    float bw = 0.005*(iResolution.y-(iResolution.x/16.0*9.0));
    if ( any( bvec2( iPosition.y < -1.0+bw, iPosition.y > 1.0-bw) ) ) col = vec3( 0.0 );

	gl_FragColor = vec4( col, 1.0 );
}