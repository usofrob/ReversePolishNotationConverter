node {
	stage 'Install'
		./scripts/installEnvironment.sh
		sh 'make clean'
		
	stage 'Build'
		sh 'make all'
		
	stage 'Test'
		sh 'make test'
}
