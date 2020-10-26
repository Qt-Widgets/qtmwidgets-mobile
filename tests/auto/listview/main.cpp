
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>

// QtMWidgets include.
#include <QtMWidgets/AbstractListView>
#include <QtMWidgets/AbstractListModel>


class ListView
	:	public QtMWidgets::AbstractListView< QColor >
{
public:
	explicit ListView( QWidget * parent = nullptr )
		:	QtMWidgets::AbstractListView< QColor > ( parent )
	{
		setModel( new QtMWidgets::ListModel< QColor > () );
	}

protected:
	void drawRow( QPainter * painter,
		const QRect & rect, int row )
	{
		const QColor & c = model()->data( row );

		painter->setPen( Qt::black );
		painter->setBrush( c );

		painter->drawRect( rect );

		QTextOption opt;
		opt.setAlignment( Qt::AlignCenter );

		painter->drawText( rect, QString::number( row ), opt );
	}
};


class TestListView
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_w.reset( new ListView );

		m_data.append( Qt::blue );
		m_data.append( Qt::red );
		m_data.append( Qt::green );
		m_data.append( Qt::cyan );
		m_data.append( Qt::magenta );
		m_data.append( Qt::gray );
		m_data.append( Qt::yellow );
		m_data.append( Qt::white );

		m_data.append( Qt::blue );
		m_data.append( Qt::red );
		m_data.append( Qt::green );
		m_data.append( Qt::cyan );
		m_data.append( Qt::magenta );
		m_data.append( Qt::gray );
		m_data.append( Qt::yellow );
		m_data.append( Qt::white );

		for( int i = 0; i < m_data.size(); ++i )
			m_w->model()->appendRow( m_data.at( i ) );

		m_w->resize( 100, 200 );
		m_w->show();
	}

	void testListModel()
	{
		QVERIFY( QTest::qWaitForWindowActive( m_w.data() ) );

		m_w->model()->insertRow( m_w->model()->rowCount() );

		QVERIFY( m_w->model()->data( m_w->model()->rowCount() - 1 ) == QColor() );

		m_w->model()->moveRow( m_w->model()->rowCount() - 1, m_w->model()->rowCount() - 2 );

		QVERIFY( m_w->model()->data( m_w->model()->rowCount() - 1 ) ==
			m_data.back() );

		m_w->model()->removeRow( m_w->model()->rowCount() - 2 );

		QVERIFY( m_w->model()->rowCount() == m_data.size() );

		m_w->model()->moveRows( 0, 2, m_w->model()->rowCount() - 1 );

		QVERIFY( m_w->model()->data( m_w->model()->rowCount() - 2 ) == m_data.at( 0 ) );
		QVERIFY( m_w->model()->data( m_w->model()->rowCount() - 1 ) == m_data.at( 1 ) );

		m_w->model()->moveRows( m_w->model()->rowCount() - 2, 2, 0 );

		QVERIFY( m_w->model()->data( 0 ) == m_data.at( 0 ) );
		QVERIFY( m_w->model()->data( 1 ) == m_data.at( 1 ) );

		QVERIFY( m_w->model()->data( m_w->model()->rowCount() - 1 ) == m_data.back() );

		m_w->model()->setData( 1, QColor() );

		QVERIFY( m_w->model()->data( 1 ) == QColor() );

		m_w->model()->reset();

		QVERIFY( m_w->model()->rowCount() == 0 );

		for( int i = 0; i < m_data.size(); ++i )
			m_w->model()->appendRow( m_data.at( i ) );

		QVERIFY( m_w->model()->rowCount() == m_data.size() );
		QVERIFY( m_w->model()->data( m_data.size() - 1 ) == m_data.back() );
	}

private:
	QSharedPointer< ListView > m_w;
	QVector< QColor > m_data;
};


QTEST_MAIN( TestListView )

#include "main.moc"
