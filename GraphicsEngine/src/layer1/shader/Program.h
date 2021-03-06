/*
 * Program.h
 *
 *  Created on: 26.04.2011
 *      Author: Norbert Nopper
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <map>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "GL/glus.h"

class Program
{

protected:

	static Program* lastUsedProgram;

	GLUSshaderprogram shaderprogram;

	std::string type;

	std::string computeFilename;
	std::string vertexFilename;
	std::string controlFilename;
	std::string evaluationFilename;
	std::string geometryFilename;
	std::string fragmentFilename;

	std::map<std::string, boost::int32_t> allUniforms;
	std::map<std::string, boost::int32_t> allAtribbs;

public:

	static void off();

	Program(const std::string& type, const std::string& computeFilename);
	Program(const std::string& type, const std::string& vertexFilename, const std::string& fragmentFilename);
	Program(const std::string& type, const std::string& vertexFilename, const std::string& geometryFilename, const std::string& fragmentFilename);
	Program(const std::string& type, const std::string& vertexFilename, const std::string& controlFilename, const std::string& evaluationFilename, const std::string& geometryFilename, const std::string& fragmentFilename);
	virtual ~Program();

	bool operator ==(const Program& other) const;

	void use();

	boost::int32_t getUniformLocation(const std::string& name);
	boost::int32_t getAttribLocation(const std::string& name);

	const std::string& getType() const;

	const std::string& getComputeFilename() const;

	const std::string& getVertexFilename() const;

	const std::string& getFragmentFilename() const;

	const std::string& getControlFilename() const;

	const std::string& getEvaluationFilename() const;

	const std::string& getGeometryFilename() const;
};

typedef boost::shared_ptr<Program> ProgramSP;

#endif /* PROGRAM_H_ */
