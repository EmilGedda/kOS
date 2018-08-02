#!groovy

pipeline {
    agent {
        label 'docker-kos'
    }
    stages {
        stage('Generate build system') {
            steps {
                sh '''cd build && CC=clang cmake ..'''
            }
        }
        stage('Building kos.iso') {
            steps {
                sh '''make kos.iso'''
            }
        }
    }
}
