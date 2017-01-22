import jenkins.model.*
import hudson.model.Run
import hudson.EnvVars
import org.jenkinsci.plugins.workflow.support.steps.build.*

node {
	stage ('Install') {
		checkout scm
		sh './scripts/installEnvironment.sh'
		sh 'make clean'
	}
	stage ('Build') {
		sh 'make all'
	}
	stage ('Test') {
		sh 'make test'
	}
	stage ('MemCheck') {
		sh 'make mem-leak-check'
	}
}
