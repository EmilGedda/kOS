#!groovy

pipeline {
    agent {
        label 'docker-kos'
    }
    stages {
        stage('Generate build system') {
            steps {
                sh '''mkdir build && cd build && CC=clang cmake ..'''
            }
        }
        stage('Building kos.iso') {
            steps {
                sh '''make kos.iso'''
            }
        }
    }
}
