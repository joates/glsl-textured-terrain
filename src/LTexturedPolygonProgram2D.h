/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

#ifndef LTEXTURED_POLYGON_PROGRAM_2D_H
#define LTEXTURED_POLYGON_PROGRAM_2D_H

#include "LShaderProgram.h"
#include <glm/glm.hpp>
#include "LColorRGBA.h"
#include <math.h>

class LTexturedPolygonProgram2D : public LShaderProgram
{
	public:
		LTexturedPolygonProgram2D();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Initializes variables
        Side Effects:
         -None
        */

		bool loadProgram();
        /*
        Pre Condition:
         -A valid OpenGL context
        Post Condition:
         -Loads textured polygon program
        Side Effects:
         -None
        */

		void setVertexPointer( GLsizei stride, const GLvoid* data );
        /*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Sets vertex position attribute pointer
        Side Effects:
         -None
        */

		void setTexCoordPointer( GLsizei stride, const GLvoid* data );
        /*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Sets texture coordinate attribute pointer
        Side Effects:
         -None
        */

		void enableVertexPointer();
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Enables vertex position attribute
        Side Effects:
         -None
        */

		void disableVertexPointer();
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Disables vertex position attribute
        Side Effects:
         -None
        */

		void enableTexCoordPointer();
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Enables texture coordinate attribute
        Side Effects:
         -None
        */

		void disableTexCoordPointer();
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Enables texture coordinate attribute
        Side Effects:
         -None
        */

		void setProjection( glm::mat4 matrix );
        /*
        Pre Condition:
         -None
        Post Condition:
         -Sets member projection matrix
        Side Effects:
         -None
        */

		void setModelView( glm::mat4 matrix );
		/*
        Pre Condition:
         -None
        Post Condition:
         -Sets member modelview matrix
        Side Effects:
         -None
        */

		void leftMultProjection( glm::mat4 matrix );
		/*
        Pre Condition:
         -None
        Post Condition:
         -Left multiplies member projection matrix
        Side Effects:
         -None
        */

		void leftMultModelView( glm::mat4 matrix );
		/*
        Pre Condition:
         -None
        Post Condition:
         -Left multiplies member modelview matrix
        Side Effects:
         -None
        */

		void updateProjection();
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Updates shader program projection matrix with member projection matrix
        Side Effects:
         -None
        */

		void updateModelView();
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Updates shader program modelview matrix with member modelview matrix
        Side Effects:
         -None
        */

		void setTextureColor( LColorRGBA color );
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Updates shader program textured polygon color
        Side Effects:
         -None
        */

		void setTextureUnit( GLuint unit );
		/*
        Pre Condition:
         -Bound LTexturedPolygonProgram2D
        Post Condition:
         -Updates shader program multitexture unit
        Side Effects:
         -None
        */

                void setResolution( GLfloat w, GLfloat h );

                void setGlobalTime( GLfloat t );

                void setCameraAngle( GLfloat x );

	private:
		//Attribute locations
		GLint mVertexPos2DLocation;
		GLint mTexCoordLocation;

		//Coloring location
		GLint mTextureColorLocation;

		//Texture unit location
		GLint mTextureUnitLocation;

		//Projection matrix
		glm::mat4 mProjectionMatrix;
		GLint mProjectionMatrixLocation;

		//Modelview matrix
		glm::mat4 mModelViewMatrix;
		GLint mModelViewMatrixLocation;

                GLint mResolutionLocation;
                GLint mGlobalTimeLocation;

                GLfloat mCameraAngleLocation;
};

#endif
